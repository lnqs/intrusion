#ifndef TEXTRENDER_H
#define TEXTRENDER_H

#include "defines.h"
#include "clib.h"
#include "textrender_glyphs.h"

static unsigned char textrender_buffer[RESOLUTION_X * RESOLUTION_Y];

static stdcall bool is_part_of_glyph(uint32_t glyph, size_t x, size_t y)
{
    return (0x1 << (y * glyph_width + x)) & glyph;
}

static stdcall void set_pixel(size_t x, size_t y, unsigned char intensity)
{
    textrender_buffer[y * RESOLUTION_X + x] = 0xff;
}

static stdcall void render_character(char c, size_t x, size_t y)
{
    const float scaling_factor_w = GLYPH_SCALE * (RESOLUTION_X / 800.0);
    const float scaling_factor_h = GLYPH_SCALE * (RESOLUTION_Y / 600.0);

    const uint32_t glyph = glyphs[c - glyphs_ascii_begin];

    for (size_t cy = 0; cy < glyph_height * scaling_factor_h; cy++)
    {
        for (size_t cx = 0; cx < glyph_width * scaling_factor_w; cx++)
        {
            if (is_part_of_glyph(glyph, cx / scaling_factor_w, cy / scaling_factor_h))
            {
                set_pixel(cx + x, cy + y, 0xff);
            }
        }
    }
}

static stdcall void set_text(const char* text)
{
    for (size_t i = 0; i < RESOLUTION_X * RESOLUTION_Y; i++)
    {
        textrender_buffer[i] = 0x0;
    }

    size_t x = TEXT_START_X;
    size_t y = TEXT_START_Y;
    for (const char* c = text; *c != '\0'; c++)
    {
        if (*c == '\n')
        {
            x = TEXT_START_X;
            y += glyph_height * GLYPH_SCALE + GLYPH_SPACING;
        }
        else
        {
            render_character(*c, x, y);
            x += glyph_width * GLYPH_SCALE + GLYPH_SPACING;
        }
    }
}

#endif

