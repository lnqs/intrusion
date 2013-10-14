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

static const char* gl_symbols[] = {
    "glMatrixMode",
    "glOrtho",
    "glBegin",
    "glEnd",
    "glVertex3f",
    "glCreateShader",
    "glShaderSource",
    "glCompileShader",
    "glAttachShader",
    "glCreateProgram",
    "glLinkProgram",
    "glUseProgram",
    "glGetUniformLocation",
    "glUniform3fv",
    "glUniformMatrix3fv"
};

static const void* gl_functions[sizeof(gl_symbols) / sizeof(const char*)];

static stdcall void initialize_gl_functions()
{
    // we have to import by name instead of hash, since the nvidia-GL-library
    // is using the old sysv-style hash-tables
    for (int i = 0; i < sizeof(gl_symbols) / sizeof(const char*); i++)
    {
        gl_functions[i] = SDL_GL_GetProcAddress_fn(gl_symbols[i]);
    }
}

#endif

