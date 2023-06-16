#include <string.h>
#include <foam/core/physics/collision.h>
#include <foam/core/ext/logger.h>

void
fe_plane_init(fe_plane_t *plane, const fe_vec3_t *origin, const fe_vec3_t *normal)
{
    plane->origin = *origin;
    plane->normal = *normal;
    plane->equation.x = normal->x;
    plane->equation.y = normal->y;
    plane->equation.z = normal->z;
    plane->equation.w = -(normal->x * origin->x + normal->y * origin->y + normal->z * origin->z);
}

void
fe_plane_init_from_triangle(fe_plane_t *plane, const fe_vec3_t *a, const fe_vec3_t *b, const fe_vec3_t *c)
{
    fe_vec3_t ba, ca;
    fe_vec3_sub_to(&ba, b, a);
    fe_vec3_sub_to(&ca, c, a);

    fe_vec3_t tmp;
    fe_vec3_cross_to(&tmp, &ba, &ca);
    fe_vec3_norm(&tmp);

    plane->origin = *a;
    plane->normal = tmp;

    plane->equation.x = tmp.x;
    plane->equation.y = tmp.y;
    plane->equation.z = tmp.z;
    plane->equation.w = -(tmp.x * a->x + tmp.y * a->y + tmp.z * a->z);
}

f32 
fe_plane_signed_distance(const fe_plane_t *plane, const fe_vec3_t *base_point)
{
    return fe_vec3_dot(base_point, &plane->normal) + plane->equation.w;
}

bool 
fe_plane_is_front_facing(const fe_plane_t *plane, const fe_vec3_t *direction)
{
    f64 f = fe_vec3_dot(&plane->normal, direction);

    if (f <= 0.0) return true;

    return false;
}

bool 
fe_check_point_in_triangle(const fe_vec3_t *point, const fe_vec3_t *p1, const fe_vec3_t *p2, const fe_vec3_t *p3)
{
    fe_vec3_t u, v, w, vw, vu, uw, uv;
    fe_vec3_sub_to(&u, p2, p1);
    fe_vec3_sub_to(&v, p3, p1);
    fe_vec3_sub_to(&w, point, p1);

    fe_vec3_cross_to(&vw, &v, &w);
    fe_vec3_cross_to(&vu, &v, &u);

    if (fe_vec3_dot(&vw, &vu) < 0.0f) return false;

    fe_vec3_cross_to(&uw, &u, &w);
    fe_vec3_cross_to(&uv, &u, &v);

    if (fe_vec3_dot(&uw, &uv) < 0.0f) return false;

    f32 d = fe_vec3_len(&uv);
    f32 r = fe_vec3_len(&vw) / d;
    f32 t = fe_vec3_len(&uw) / d;

    return ((r + t) <= 1.0f);
}

bool 
fe_get_lowest_root(f32 a, f32 b, f32 c, f32 max, f32 *root)
{
    // check if solution exists
    f32 determinant = b*b - 4.0f*a*c;

    // if negative there is no solution
    if (determinant < 0.0f)
    return false;

    // calculate two roots
    f32 sqrtD = sqrtf(determinant);
    f32 r1 = (-b - sqrtD) / (2.0f*a);
    f32 r2 = (-b + sqrtD) / (2.0f*a);

    // set x1 <= x2
    if (r1 > r2) 
    {
        f32 tmp = r2;
        r2 = r1;
        r1 = tmp;
    }

    // get lowest root
    if (r1 > 0 && r1 < max) 
    {
        *root = r1;
        return true;
    }

    if (r2 > 0 && r2 < max) 
    {
        *root = r2;
        return true;
    }

    // no solutions
    return false;
}

void 
fe_collision_check_triangle(fe_collision_t *collision, const fe_vec3_t *p1, const fe_vec3_t *p2, const fe_vec3_t *p3)
{
    fe_plane_t plane;
    fe_plane_init_from_triangle(&plane, p1, p2, p3);

    // only check front facing triangles
    if (!fe_plane_is_front_facing(&plane, &collision->e_norm_velocity)) return;

    // get interval of plane intersection
    f64 t0, t1;
    bool embedded_in_plane = false;

    // signed distance from sphere to point on plane
    f64 signed_dist_to_plane = fe_plane_signed_distance(&plane, &collision->e_base_point);

    // cache this as we will reuse
    f32 normal_dot_vel = fe_vec3_dot(&plane.normal, &collision->e_velocity);

    // if sphere is moving parrallel to plane
    if (normal_dot_vel == 0.0f) 
    {
        if (fabs(signed_dist_to_plane) >= 1.0f) 
        {
            // no collision possible 
            return;
        } 
        else 
        {
            // sphere is in plane in whole range [0..1]
            embedded_in_plane = true;
            t0 = 0.0;
            t1 = 1.0;
        }
    } 
    else 
    {
        // N dot D is not 0, calc intersect interval
        // f32 nvi = 1.0f / normal_dot_vel;
        t0 = (-1.0 - signed_dist_to_plane) / normal_dot_vel;
        t1 = ( 1.0 - signed_dist_to_plane) / normal_dot_vel;

        // swap so t0 < t1
        if (t0 > t1) 
        {
            f64 tmp = t1;
            t1 = t0;
            t0 = tmp;
        }

        // check that at least one result is within range
        if (t0 > 1.0 || t1 < 0.0) 
        {
            // both values outside range [0,1] so no collision
            return;
        }

        // clamp to [0,1]
        if (t0 < 0.0) t0 = 0.0;
        if (t1 < 0.0) t1 = 0.0;
        if (t0 > 1.0) t0 = 1.0;
        if (t1 > 1.0) t1 = 1.0;
    }

    // time to check for a collision
    fe_vec3_t collision_point = FE_VEC3_ZERO;
    bool found_collision = false;
    f64 t = 1.0;

    // first check collision with the inside of the triangle
    if (!embedded_in_plane) 
    {
        fe_vec3_t plane_intersect, tmp;
        fe_vec3_sub_to(&plane_intersect, &collision->e_base_point, &plane.normal);
        fe_vec3_scale_to(&tmp, &collision->e_velocity, t0);
        fe_vec3_add_to(&plane_intersect, &plane_intersect, &tmp);

        if (fe_check_point_in_triangle(&plane_intersect, p1, p2, p3)) 
        {
            found_collision = true;
            t = t0;
            collision_point = plane_intersect;
        }
    }

    // no collision yet, check against points and edges
    if (!found_collision) 
    {
        fe_vec3_t velocity = collision->e_velocity;
        fe_vec3_t base = collision->e_base_point;

        f32 velocity_sqrt_length = fe_vec3_dot(&velocity, &velocity);
        f32 a = 0, b = 0, c = 0;
        f32 new_t = 0;

        // equation is a*t^2 + b*t + c = 0
        // check against points
        a = velocity_sqrt_length;

        // p1
        fe_vec3_t tmp;
        fe_vec3_sub_to(&tmp, &base, p1);
        b = 2.0f * (fe_vec3_dot(&velocity, &tmp));
        fe_vec3_sub_to(&tmp, p1, &base);
        c = fe_vec3_dot(&tmp, &tmp) - 1.0;
        if (fe_get_lowest_root(a, b, c, t, &new_t)) 
        {
            t = new_t;
            found_collision = true;
            collision_point = *p1;
        }

        // p2
        if (!found_collision) 
        {
            fe_vec3_sub_to(&tmp, &base, p2);
            b = 2.0f * (fe_vec3_dot(&velocity, &tmp));
            fe_vec3_sub_to(&tmp, p2, &base);
            c = fe_vec3_dot(&tmp, &tmp) - 1.0;
            if (fe_get_lowest_root(a, b, c, t, &new_t)) 
            {
                t = new_t;
                found_collision = true;
                collision_point = *p2;
            }
        }

        // p3
        if (!found_collision) 
        {
            fe_vec3_sub_to(&tmp, &base, p3);
            b = 2.0f * (fe_vec3_dot(&velocity, &tmp));
            fe_vec3_sub_to(&tmp, p3, &base);
            c = fe_vec3_dot(&tmp, &tmp) - 1.0;
            if (fe_get_lowest_root(a, b, c, t, &new_t)) 
            {
                t = new_t;
                found_collision = true;
                collision_point = *p3;
            }
        }

        // check against edges
        // p1 -> p2
        fe_vec3_t edge, base_to_vertex;
        fe_vec3_sub_to(&edge, p2, p1);
        fe_vec3_sub_to(&base_to_vertex, p1, &base);
        f32 edge_sqrt_length        = fe_vec3_dot(&edge, &edge);
        f32 edge_dot_velocity       = fe_vec3_dot(&edge, &velocity);
        f32 edge_dot_base_to_vertex = fe_vec3_dot(&edge, &base_to_vertex);

        // calculate params for equation
        a = edge_sqrt_length * -velocity_sqrt_length + edge_dot_velocity * edge_dot_velocity;
        b = edge_sqrt_length * (2.0f * fe_vec3_dot(&velocity, &base_to_vertex)) -
            2.0f * edge_dot_velocity * edge_dot_base_to_vertex;
        c = edge_sqrt_length * (1.0f - fe_vec3_dot(&base_to_vertex, &base_to_vertex)) +
            edge_dot_base_to_vertex * edge_dot_base_to_vertex;

        // do we collide against infinite edge
        if (fe_get_lowest_root(a, b, c, t, &new_t)) 
        {
            // check if intersect is within line segment
            f32 f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_sqrt_length;
            if (f >= 0.0f && f <= 1.0f) 
            {
                t = new_t;
                found_collision = true;
                fe_vec3_scale_to(&tmp, &edge, f);
                fe_vec3_add(&tmp, p1);
                collision_point = tmp;
            }
        }


        // p2 -> p3
        fe_vec3_sub_to(&edge, p3, p2);
        fe_vec3_sub_to(&base_to_vertex, p2, &base);
        edge_sqrt_length        = fe_vec3_dot(&edge, &edge);
        edge_dot_velocity       = fe_vec3_dot(&edge, &velocity);
        edge_dot_base_to_vertex = fe_vec3_dot(&edge, &base_to_vertex);

        // calculate params for equation
        a = edge_sqrt_length * -velocity_sqrt_length + edge_dot_velocity * edge_dot_velocity;
        b = edge_sqrt_length * (2.0f * fe_vec3_dot(&velocity, &base_to_vertex)) -
            2.0f * edge_dot_velocity * edge_dot_base_to_vertex;
        c = edge_sqrt_length * (1.0f - fe_vec3_dot(&base_to_vertex, &base_to_vertex)) +
            edge_dot_base_to_vertex * edge_dot_base_to_vertex;

        // do we collide against infinite edge
        if (fe_get_lowest_root(a, b, c, t, &new_t)) 
        {
            // check if intersect is within line segment
            f32 f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_sqrt_length;
            if (f >= 0.0f && f <= 1.0f) 
            {
                t = new_t;
                found_collision = true;
                fe_vec3_scale_to(&tmp, &edge, f);
                fe_vec3_add(&tmp, p2);
                collision_point = tmp;
            }
        }


        // p3 -> p1
        fe_vec3_sub_to(&edge, p1, p3);
        fe_vec3_sub_to(&base_to_vertex, p3, &base);
        edge_sqrt_length        = fe_vec3_dot(&edge, &edge);
        edge_dot_velocity       = fe_vec3_dot(&edge, &velocity);
        edge_dot_base_to_vertex = fe_vec3_dot(&edge, &base_to_vertex);

        // calculate params for equation
        a = edge_sqrt_length * -velocity_sqrt_length + edge_dot_velocity * edge_dot_velocity;
        b = edge_sqrt_length * (2.0f * fe_vec3_dot(&velocity, &base_to_vertex)) -
            2.0f * edge_dot_velocity * edge_dot_base_to_vertex;
        c = edge_sqrt_length * (1.0f - fe_vec3_dot(&base_to_vertex, &base_to_vertex)) +
            edge_dot_base_to_vertex * edge_dot_base_to_vertex;

        // do we collide against infinite edge
        if (fe_get_lowest_root(a, b, c, t, &new_t)) 
        {
            // check if intersect is within line segment
            f32 f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_sqrt_length;
            if (f >= 0.0f && f <= 1.0f) 
            {
                t = new_t;
                found_collision = true;
                fe_vec3_scale_to(&tmp, &edge, f);
                fe_vec3_add(&tmp, p3);
                collision_point = tmp;
            }
        }
    }

    // set results
    if (found_collision) 
    {
        // distance to collision, t is time of collision
        f64 dist_to_coll = t * fe_vec3_len(&collision->e_velocity);

        // are we the closest hit?
        if (!collision->found_collision || dist_to_coll < collision->nearest_distance) 
        {
            collision->nearest_distance = dist_to_coll;
            collision->intersect_point = collision_point;
            collision->found_collision = true;
            collision->t = t;
            collision->plane = plane;
            collision->a = *p1;
            collision->b = *p2;
            collision->c = *p3;
        }
    }
}

