#include <foam/core/data/font.h>
#include <foam/core/data/font_data.h>

FE_RETURN_CODE 
fe_font_make_from_file(fe_font_t *font, const char *path, const fe_vec2_t *character_size)
{
    FE_RETURN_CODE rc = fe_reader_make_from_file(&font->data, path);
    if (rc != FE_OK) return rc;

    font->character_size = *character_size;

    return rc;
}

void 
fe_font_make_from_buffer(fe_font_t *font, const u8 *buffer, size_t buf_len, const fe_vec2_t *character_size)
{
    fe_reader_make_from_buffer(&font->data, buffer, buf_len);
    font->character_size = *character_size;
}

fe_texture_t
fe_font_make_texture(const fe_font_t *font, const char *str, const fe_vec2_t *texture_size, const fe_color_t *fg_color, const fe_color_t *bg_color)
{
    fe_texture_t tex;
    fe_texture_make(&tex, texture_size);
    fe_font_print_on_texture(font, &tex, str, &fe_vec2(0, 0), fg_color, bg_color);

    return tex;
}

void
fe_font_print_on_texture(const fe_font_t *font, fe_texture_t *tex, const char *str, const fe_vec2_t *pos, const fe_color_t *fg, const fe_color_t *bg)
{
    size_t tx = tex->size.x;
    size_t ty = tex->size.y;
    size_t csx = font->character_size.x;
    size_t csy = font->character_size.y;
    size_t x = (size_t)(pos->x * font->character_size.x);
    size_t y = (size_t)(pos->y * font->character_size.y);

    for (size_t i = 0; i < strlen(str); i++) 
    {
        if (str[i] == '\n')
        {
            y+= csy;
            continue;
        }

        size_t c = (str[i] * 16);

        if (c > 4096) c = 0;

        if (x >= tx) 
        {
            x = 0;
            y += csy;
        }

        if (y >= ty) y = 0;

        for (u8 j = 0; j < csy; j++) 
        {
            size_t p = (tx * y) + x;
            p       += (tx * j);

            u8 byte = FE_FONT_XGA_8x16[c+j];

            for (u8 k = 0; k < csx; k++) 
            {
                size_t pixel = p + (8 - k);
                if (pixel >= (tex->size.x * tex->size.y)) continue;

                int color = (byte >> k) & 0x01;
                if (color)  tex->pixels[pixel] = *fg;
                else        tex->pixels[pixel] = *bg;
            }
        }

        x += csx;
    }
}

void
fe_font_kill(fe_font_t *font)
{
    fe_reader_kill(&font->data);
}
