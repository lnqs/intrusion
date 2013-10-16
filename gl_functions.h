#ifndef GL_FUNCTIONS_H
#define GL_FUNCTIONS_H

#include <GL/gl.h>
#include <SDL.h>
#include "sdl_functions.h"

#define gl_call(name, type) ((type)gl_functions[name ## _i])

#define glMatrixMode_fn \
    gl_call(glMatrixMode, void GLAPIENTRY (*)(GLenum))

#define glOrtho_fn \
    gl_call(glOrtho, void GLAPIENTRY (*)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))

#define glBegin_fn \
    gl_call(glBegin, void GLAPIENTRY (*)(GLenum))

#define glEnd_fn \
    gl_call(glEnd, void GLAPIENTRY (*)())

#define glVertex3f_fn \
    gl_call(glVertex3f, void GLAPIENTRY (*)(GLfloat, GLfloat, GLfloat))

#define glCreateShader_fn \
    gl_call(glCreateShader, GLuint GLAPIENTRY (*)(GLenum))

#define glShaderSource_fn \
    gl_call(glShaderSource, void GLAPIENTRY (*)(GLuint, GLsizei count, const GLchar**, const GLint*))

#define glCompileShader_fn \
    gl_call(glCompileShader, void GLAPIENTRY (*)(GLuint))

#define glAttachShader_fn \
    gl_call(glAttachShader, void GLAPIENTRY (*)(GLuint, GLuint))

#define glCreateProgram_fn \
    gl_call(glCreateProgram, GLuint GLAPIENTRY (*)())

#define glLinkProgram_fn \
    gl_call(glLinkProgram, void GLAPIENTRY (*)(GLuint))

#define glUseProgram_fn \
    gl_call(glUseProgram, void GLAPIENTRY (*)(GLuint))

#define glGetUniformLocation_fn \
    gl_call(glGetUniformLocation, GLint GLAPIENTRY (*)(GLuint, const GLchar*))

#define glUniform3fv_fn \
    gl_call(glUniform3fv, void GLAPIENTRY (*)(GLint, GLsizei, const GLfloat*))

#define glUniformMatrix3fv_fn \
    gl_call(glUniformMatrix3fv, void GLAPIENTRY (*)(GLint, GLsizei, GLboolean, const GLfloat*))

enum {
    glMatrixMode_i,
    glOrtho_i,
    glBegin_i,
    glEnd_i,
    glVertex3f_i,
    glCreateShader_i,
    glShaderSource_i,
    glCompileShader_i,
    glAttachShader_i,
    glCreateProgram_i,
    glLinkProgram_i,
    glUseProgram_i,
    glGetUniformLocation_i,
    glUniform3fv_i,
    glUniformMatrix3fv_i
};

static uint32_t gl_hashes[] = {
    0xfad70f52, // glMatrixMode
    0xfe314144, // glOrtho
    0xfd3eaa9d, // glBegin
    0x0f83490f, // glEnd
    0xc99e058f, // glVertex3f
    0x835cdd03, // glCreateShader
    0xbba22800, // glShaderSource
    0x712f7898, // glCompileShader
    0x9f5da104, // glAttachShader
    0x205c8f24, // glCreateProgram
    0x2fed8c1e, // glLinkProgram
    0x4f3ddefd, // glUseProgram
    0x4c1fa891, // glGetUniformLocation
    0x509144a7, // glUniform3fv
    0x17b296bc  // glUniformMatrix3fv
};

static const void* gl_functions[sizeof(gl_hashes) / sizeof(uint32_t)];

static stdcall void initialize_gl_functions()
{
    for (int i = 0; i < sizeof(gl_hashes) / sizeof(uint32_t); i++)
    {
        // TODO: Constant. And something to strip the version-number.
        gl_functions[i] = resolve_symbol("libGL.so.1", gl_hashes[i]);
    }
}

#endif

