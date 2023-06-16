#include <foam/core/data/font.h>
#include <foam/client/gfx/renderer.h>
#include <foam/client/ecs/components/text2d.h>

void 
fe_text2d_make(fe_text2d_t *text, const fe_font_t *font, const fe_vec2_t *tex_size, const fe_color_t *fg_color, const fe_color_t *bg_color)
{
    text->font       = font;
    text->fg_color   = *fg_color;
    text->bg_color   = *bg_color;
    fe_texture_make(&text->tex, tex_size);
    fe_gpu_texture_make_from_texture(&text->gpu_tex, &text->tex);
    text->dirty      = true;
}

void           
fe_text2d_kill(fe_text2d_t *text)
{
    fe_texture_kill(&text->tex);
    fe_gpu_texture_kill(&text->gpu_tex);
}

void
fe_text2d_resize_texture(fe_text2d_t *text, const fe_vec2_t *size)
{
    fe_texture_kill(&text->tex);
    fe_texture_make(&text->tex, size);
    text->dirty = true;
}

void
fe_text2d_print(fe_text2d_t *text, const char *str, const fe_vec2_t *pos)
{
    fe_font_print_on_texture(text->font, &text->tex, str, pos, &text->fg_color, &text->bg_color);
    text->dirty = true;
}

void           
fe_text2d_clear(fe_text2d_t *text)
{
    fe_texture_clear_pixels(&text->tex);
    text->dirty = true;
}
