#ifndef TEXTRENDER_H
#define TEXTRENDER_H

#include "defines.h"
#include "clib.h"
#include "gl_functions.h"
#include "shader.h"

static GLuint textrender_texture;
static unsigned char textrender_buffer[RESOLUTION_X * RESOLUTION_Y];

static stdcall void initialize_textrender(GLint program)
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, textrender_texture);
    gl.glGenTextures(1, &textrender_texture);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            RESOLUTION_X, RESOLUTION_Y, 0, GL_RED, GL_UNSIGNED_BYTE, textrender_buffer);

    uniform_int(program, "t", 0);

    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

#endif

