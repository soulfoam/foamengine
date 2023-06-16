#include <foam/core/return_code.h>
#include <foam/core/data/font.h>
#include <foam/client/resource.h>
#include <foam/client/gfx/gl.h>
#include <foam/client/gfx/renderer.h>
#include <foam/client/gfx/shader.h>
#include <foam/client/gfx/gpu_mesh.h>
#include <foam/client/gfx/gpu_texture.h>
#include <foam/client/gfx/window.h>

static const char *S_MODEL      = "fe_model";
static const char *S_VIEW       = "fe_view";
static const char *S_PROJECTION = "fe_projection";
static const char *S_COLOR      = "fe_color";

#define ACTIVE_SHADER_GL_ID fe_renderer_get_shader(fe_renderer_get_active_shader())

typedef struct
{
    u32               screen_fbo;
    u32               screen_rbo;
    fe_gpu_texture_t  screen_tex;

    fe_vec(FE_SHADER_TYPE) shaders;
    FE_SHADER_TYPE         active_shader;

    fe_vec(fe_mat4x4_t)    view_stack;
    fe_vec(fe_mat4x4_t)    projection_stack;

    //for 2d sprite rendering
    u32 quad_vao, quad_vbo;

    fe_color_t clear_color;

} fe_renderer_t;

static fe_renderer_t rndr;

static void
apply_view_matrix(void)
{
    fe_shader_get_location_id s_id = fe_shader_get_location(ACTIVE_SHADER_GL_ID, S_VIEW);
    if (s_id == -1) return;

    if (fe_vec_is_empty(rndr.view_stack))
    {
        fe_mat4x4_t view;
        fe_mat4x4_identity(&view);
        fe_shader_set_mat4x4(s_id, &view);
    }
    else
    {
        fe_shader_set_mat4x4(s_id, &rndr.view_stack[fe_vec_get_last_index(rndr.view_stack)]);
    }
}

static void
apply_projection_matrix(void)
{
    fe_shader_get_location_id p_id = fe_shader_get_location(ACTIVE_SHADER_GL_ID, S_PROJECTION);
    if (p_id == -1) return;

    if (fe_vec_is_empty(rndr.projection_stack))
    {
        fe_mat4x4_t proj;
        fe_mat4x4_identity(&proj);
        fe_shader_set_mat4x4(p_id, &proj);
    }
    else
    {
        fe_shader_set_mat4x4(p_id, &rndr.projection_stack[fe_vec_get_last_index(rndr.projection_stack)]);
    }
}

static void
setup_quad(void)
{
    f32 vertices[] = 
    { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &rndr.quad_vao);
    glGenBuffers(1, &rndr.quad_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, rndr.quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(rndr.quad_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

static bool
renderer_screen_setup(const fe_vec2_t *screen_tex_size)
{
    fe_gpu_texture_make_fbo(&rndr.screen_tex, screen_tex_size, "screen_fbo");

    glGenFramebuffers(1, &rndr.screen_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, rndr.screen_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rndr.screen_tex.id, 0);
    glGenRenderbuffers(1, &rndr.screen_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rndr.screen_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_tex_size->x, screen_tex_size->y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rndr.screen_rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fe_log_error("Failed to initialize framebuffer");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

static void
renderer_screen_cleanup(void)
{
    glDeleteFramebuffers(1, &rndr.screen_fbo);
    glDeleteRenderbuffers(1, &rndr.screen_rbo);
}


FE_RETURN_CODE
fe_renderer_setup(const fe_vec2_t *render_resolution, u32 shader_count)
{	
    fe_vec_make_with_count(rndr.shaders, shader_count);
    rndr.active_shader = FE_SHADER_COUNT;

    rndr.projection_stack = FE_VEC_EMPTY;
    rndr.view_stack       = FE_VEC_EMPTY;

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    rndr.clear_color = fe_color_rgb(0, 0, 0);

    setup_quad();

    renderer_screen_setup(render_resolution);

    return FE_OK;
}

void
fe_renderer_cleanup(void)
{ 
	glDeleteBuffers(1,                 &rndr.quad_vbo);
	glDeleteVertexArrays(1,            &rndr.quad_vao);
    fe_vec_kill(rndr.shaders,          fe_shader_kill);
    fe_vec_kill(rndr.view_stack,       NULL);
    fe_vec_kill(rndr.projection_stack, NULL);
    fe_gpu_texture_kill(&rndr.screen_tex);

    renderer_screen_cleanup();
}

FE_RETURN_CODE
fe_renderer_register_shader(FE_SHADER_TYPE type, const char *path)
{   
    FE_RETURN_CODE rc = FE_OK;

    fe_shader_id shader;

	fe_shader_source_t src;
    rc = fe_shader_source_make_from_file(&src, path);
    if (rc != FE_OK) goto error;

    rc = fe_shader_make(&shader, &src);
    if (rc != FE_OK) goto error;

    rndr.shaders[type] = shader;
	fe_shader_source_kill(&src);

    //clean this up, this is just because we dont embed shaders and need one to be loaded first from game side
    if (rndr.active_shader == FE_SHADER_COUNT) 
    {
        fe_renderer_bind_shader(type);
        fe_mat4x4_t iden;
        fe_mat4x4_identity(&iden);
        fe_renderer_push_view_matrix(&iden);
        fe_renderer_push_projection_matrix(&iden);
    };

    return rc;

error:
    fe_log_error("Failed to add shader '%s' | Vertex Path: '%s' | Fragment Path: '%s'");
    fe_shader_source_kill(&src);

    return FE_ERR_GL_SHADER_COMPILE;
}

fe_shader_id      
fe_renderer_get_shader(FE_SHADER_TYPE type)
{
    return rndr.shaders[type];
}

void
fe_renderer_bind_shader(FE_SHADER_TYPE type)
{
    if (rndr.active_shader == type) return;

    rndr.active_shader = type;
    fe_shader_use(fe_renderer_get_shader(type));
    apply_view_matrix();
    apply_projection_matrix();
}


FE_SHADER_TYPE
fe_renderer_get_active_shader(void)
{
	return rndr.active_shader;
}

void
fe_renderer_draw_gpu_mesh(fe_gpu_mesh_t *gpu_mesh)
{ 
    fe_gpu_mesh_upload_instances(gpu_mesh);
    fe_gpu_mesh_draw(gpu_mesh, ACTIVE_SHADER_GL_ID);    
}

void
fe_renderer_draw_resource_gpu_meshes(void)
{ 
    fe_genvec_for_each(fe_resource_get_gpu_mesh_genvec(), gpu_mesh)
    {
        fe_renderer_draw_gpu_mesh(gpu_mesh);
    }
}

void
fe_renderer_draw_gpu_texture(fe_gpu_texture_t *tex, const fe_vec2_t *pos, const fe_quat_t *rot, const fe_vec2_t *size, const fe_color_t *color)
{
    fe_mat4x4_t mdl;
    fe_mat4x4_identity(&mdl);
    fe_mat4x4_translate(&mdl, &fe_vec3(pos->x, pos->y, 0));
    fe_mat4x4_translate(&mdl, &fe_vec3(0.5f * size->x, 0.5 * size->y, 0.0f));
    fe_mat4x4_rotate_quat(&mdl, rot);
    fe_mat4x4_translate(&mdl, &fe_vec3(-0.5f * size->x, -0.5 * size->y, 0.0f));
    fe_mat4x4_scale(&mdl, &fe_vec3(size->x, size->y, 1.0f));

    fe_shader_set_mat4x4(fe_shader_get_location(ACTIVE_SHADER_GL_ID, S_MODEL), &mdl);
    fe_shader_set_vec4(fe_shader_get_location(ACTIVE_SHADER_GL_ID, S_COLOR), &fe_vec4(color->r/255, color->g/255, color->b/255, color->a/255));

    fe_gpu_texture_bind(tex);

    glBindVertexArray(rndr.quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    fe_gpu_texture_unbind();
}

static void
renderer_set_viewport(const fe_vec2_t *size)
{
    glViewport(0, 0, size->x, size->y);
}

void
fe_renderer_start_draw(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, rndr.screen_fbo);
    glEnable(GL_DEPTH_TEST);
    fe_vec4_t c = fe_color_norm(&rndr.clear_color);
    glClearColor(c.x, c.y, c.z, c.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer_set_viewport(fe_renderer_get_resolution());
}

void
fe_renderer_end_draw(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    fe_vec4_t c = fe_color_norm(&rndr.clear_color);
    glClearColor(c.x, c.y, c.z, c.w);
	glClear(GL_COLOR_BUFFER_BIT);
    renderer_set_viewport(fe_window_get_size());
}

void
fe_renderer_render(void)
{
    const fe_vec2_t *win_size      = fe_window_get_size();
    const fe_vec2_t *renderer_size = fe_renderer_get_resolution();

    f32 dest_aspect_ratio = win_size->x      / win_size->y;
    f32 aspect_ratio      = renderer_size->x / renderer_size->y;

    f32 view_width = 0, view_height = 0;
    if (aspect_ratio > dest_aspect_ratio)    
    {	
        view_width = win_size->x;	
        view_height = view_width / aspect_ratio;    
    }    
    else    
    {	
        view_height = win_size->y;	
        view_width = view_height * aspect_ratio;    
    }

    f32 viewport_x = (win_size->x - view_width) / 2;
    //f32 viewport_y = (win_size->y - view_height) / 2;

    glDisable(GL_CULL_FACE);
    fe_renderer_bind_shader(FE_SHADER_UI);
    fe_mat4x4_t projection = fe_mat4x4_ortho(0.0f, win_size->x, win_size->y, 0.0f, -1.0f, 1.0f);
    fe_renderer_push_projection_matrix(&projection);
    fe_renderer_draw_gpu_texture(&rndr.screen_tex, &fe_vec2(viewport_x, view_height), &fe_quat(0, 0, 0, 1), &fe_vec2(view_width, -view_height), &FE_COLOR_WHITE);
    fe_renderer_pop_projection_matrix();
    glEnable(GL_CULL_FACE);

	SDL_GL_SwapWindow(fe_window_get_sdl_window());
}

void
fe_renderer_set_resolution(const fe_vec2_t *size)
{
    renderer_screen_cleanup();
    renderer_screen_setup(size);
}

const fe_vec2_t*
fe_renderer_get_resolution(void)
{
    return &rndr.screen_tex.size;
}

void
fe_renderer_push_view_matrix(const fe_mat4x4_t *src)
{
    fe_vec_add(rndr.view_stack, *src);
    apply_view_matrix();
}

void
fe_renderer_pop_view_matrix(void)
{
    fe_vec_pop(rndr.view_stack, NULL);
    apply_view_matrix();
}

const fe_mat4x4_t*
fe_renderer_get_view_matrix(void)
{
    if (fe_vec_is_empty(rndr.view_stack)) return NULL;
    return &rndr.view_stack[fe_vec_get_last_index(rndr.view_stack)];
}

void
fe_renderer_push_projection_matrix(const fe_mat4x4_t *src)
{
    fe_vec_add(rndr.projection_stack, *src);
    apply_projection_matrix();
}

void
fe_renderer_pop_projection_matrix(void)
{
    fe_vec_pop(rndr.projection_stack, NULL);
    apply_projection_matrix();
}

const fe_mat4x4_t*
fe_renderer_get_projection_matrix(void)
{
    if (fe_vec_is_empty(rndr.projection_stack)) return NULL;
    return &rndr.projection_stack[fe_vec_get_last_index(rndr.projection_stack)];
}

void
fe_renderer_set_clear_color(const fe_color_t *color)
{
    rndr.clear_color = *color;
}

const fe_color_t*
fe_renderer_get_clear_color(void)
{
    return &rndr.clear_color;
}
