#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <foam/core/data/texture.h>
#include <foam/core/data/stb_image.h>
#include <foam/core/ext/logger.h>

void
fe_texture_make(fe_texture_t *tex, const fe_vec2_t *size)
{
    tex->path        = NULL;
	tex->pixels      = malloc((size_t)(sizeof(fe_color_t) * size->x * size->y));
    tex->size        = *size;
    tex->num_of_rows = 1;
    fe_texture_clear_pixels(tex);
}

FE_RETURN_CODE
fe_texture_make_from_file(fe_texture_t *tex, const char *path)
{
	i32 width = 0; i32 height = 0; i32 comp = 0;
	tex->pixels = (fe_color_t*)stbi_load(path, &width, &height, &comp, STBI_rgb_alpha);
    tex->size   = fe_vec2(width, height);

    if (!tex->pixels)
    {
        FE_RETURN_CODE rc = FE_ERR_PATH_NOT_FOUND;
        fe_log_return_code(rc, "Failed to load texture: '%s'", path);
        return rc;
    }

    tex->path        = strdup(path);
    tex->num_of_rows = 1;

    return FE_OK;
}

void
fe_texture_make_from_buffer(fe_texture_t *tex, 
                            const char *name, 
                            const u8 *buffer, size_t buffer_len, 
                            const fe_vec2_t *size)
{
	tex->pixels = malloc(buffer_len);
    memcpy(tex->pixels, buffer, buffer_len);
    tex->size = *size;

    tex->path = name ? strdup(name) : NULL;
    tex->num_of_rows = 1;
}

FE_RETURN_CODE
fe_texture_make_atlas(fe_texture_t *tex, const char *path, u32 num_of_rows)
{
    FE_RETURN_CODE rc = FE_OK;

	rc = fe_texture_make_from_file(tex, path);
	tex->num_of_rows = num_of_rows;

    return rc;
}

void 
fe_texture_kill(fe_texture_t *tex)
{
	free(tex->path);
    free(tex->pixels);
}

void                
fe_texture_set_pixel(fe_texture_t *tex, const fe_color_t *pixel, const fe_vec2_t *pos)
{    
    if (pos->x < 0 || pos->x >= tex->size.x || pos->y < 0 || pos->y >= tex->size.y) return;
    
    tex->pixels[(size_t)(pos->x + pos->y * tex->size.x)] = *pixel;
}

fe_color_t  
fe_texture_get_pixel(const fe_texture_t *tex, const fe_vec2_t *pos)
{
    if (pos->x < 0 || pos->x >= tex->size.x || pos->y < 0 || pos->y >= tex->size.y) 
        return (fe_color_t){.r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00};
    

    return tex->pixels[(size_t)(pos->x + pos->y * tex->size.x)];
}

const fe_color_t*         
fe_texture_get_pixels(const fe_texture_t *tex)
{
    return tex->pixels;
}

void                
fe_texture_put_texture(fe_texture_t *target, const fe_texture_t *src, const fe_vec2_t *pos)
{
    for (size_t y = 0; y < src->size.y; y++)
    {
        for (size_t x = 0; x < src->size.x; x++)
        {
            fe_color_t pixel = fe_texture_get_pixel(src, &fe_vec2(x, y));
            if (pixel.a > 0)
            {
                fe_texture_set_pixel(target, &pixel, pos);
            }
        }
    } 
}

void
fe_texture_clear_pixels(fe_texture_t *tex)
{
    memset(tex->pixels, 0, (size_t)(sizeof(fe_color_t) * tex->size.x * tex->size.y));
}

const char*         
fe_texture_get_path(const fe_texture_t *tex)
{
    return tex->path;
}

fe_vec2_t           
fe_texture_get_size(const fe_texture_t *tex)
{
    return tex->size;
}

void
fe_texture_print(const fe_texture_t *tex)
{
    for (size_t y = 0; y < tex->size.y; y++)
    {
        for (size_t x = 0; x < tex->size.x; x++)
        {
            fe_color_t pixel = fe_texture_get_pixel(tex, &fe_vec2(x, y));
            fe_log("%u %u %u %u", pixel.r, pixel.g, pixel.b, pixel.a);
        }     
    }
}

