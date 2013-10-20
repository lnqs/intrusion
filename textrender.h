#ifndef TEXTRENDER_H
#define TEXTRENDER_H

#include "defines.h"
#include "clib.h"
#include "glyphs.h"

static uint8_t textrender_buffer[OVERLAY_TEXTURE_WIDTH * OVERLAY_TEXTURE_HEIGHT];
static size_t textrender_current_x;
static size_t textrender_current_y;

static stdcall bool textrender_is_part_of_glyph(uint32_t glyph, size_t x, size_t y)
{
    return (0x1 << (y * glyph_width + x)) & glyph;
}

static stdcall void textrender_set_pixel(size_t x, size_t y, uint8_t intensity)
{
    textrender_buffer[y * OVERLAY_TEXTURE_WIDTH + x] = 0xff;
}

static stdcall void textrender_render_character(char c, size_t x, size_t y)
{
    const uint32_t glyph = glyphs[c - glyphs_ascii_begin];

    for (size_t cy = 0; cy < glyph_height; cy++)
    {
        for (size_t cx = 0; cx < glyph_width; cx++)
        {
            if (textrender_is_part_of_glyph(glyph, cx, cy))
            {
                textrender_set_pixel(cx + x, cy + y, 0xff);
            }
        }
    }
}

static stdcall void textrender_clear_text()
{
    clib_inaccurate_memset(textrender_buffer, 0, sizeof(textrender_buffer));

    textrender_current_x = TEXT_START_X;
    textrender_current_y = TEXT_START_Y;
}

static stdcall void textrender_set_text(const char* text)
{
    for (const char* c = text; *c != '\0'; c++)
    {
        if (*c == '\n')
        {
            textrender_current_x = 0;
            textrender_current_y += glyph_height + GLYPH_SPACING;
        }
        else
        {
            textrender_render_character(*c, textrender_current_x, textrender_current_y);
            textrender_current_x += glyph_width + GLYPH_SPACING;
        }
    }
}

#endif

