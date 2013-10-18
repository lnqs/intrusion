#ifndef GL_FUNCTIONS_H
#define GL_FUNCTIONS_H

#include <GL/gl.h>
#include "linker.h"

// It get's funny here! This string is written to the elf-header by screw_elf_header.py,
// while the symbol for it is defined in linker.ld
extern const char _gl_library;

// the members of this struct have to have the same order as in the hashes-array!
static struct
{
    void GLAPIENTRY (*glMatrixMode)(GLenum);
    void GLAPIENTRY (*glOrtho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
    void GLAPIENTRY (*glBegin)(GLenum);
    void GLAPIENTRY (*glEnd)();
    void GLAPIENTRY (*glVertex3f)(GLfloat, GLfloat, GLfloat);
    GLuint GLAPIENTRY (*glCreateShader)(GLenum);
    void GLAPIENTRY (*glShaderSource)(GLuint, GLsizei count, const GLchar**, const GLint*);
    void GLAPIENTRY (*glCompileShader)(GLuint);
    void GLAPIENTRY (*glAttachShader)(GLuint, GLuint);
    GLuint GLAPIENTRY (*glCreateProgram)();
    void GLAPIENTRY (*glLinkProgram)(GLuint);
    void GLAPIENTRY (*glUseProgram)(GLuint);
    GLint GLAPIENTRY (*glGetUniformLocation)(GLuint, const GLchar*);
    void GLAPIENTRY (*glUniform3fv)(GLint, GLsizei, const GLfloat*);
    void GLAPIENTRY (*glUniformMatrix3fv)(GLint, GLsizei, GLboolean, const GLfloat*);
} gl;

static const uint32_t gl_hashes[] = {
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

static stdcall void initialize_gl_functions()
{
    for (int i = 0; i < sizeof(gl_hashes) / sizeof(uint32_t); i++)
    {
        ((void**)&gl)[i] = resolve_symbol(&_gl_library, gl_hashes[i]);
    }
}

#endif

