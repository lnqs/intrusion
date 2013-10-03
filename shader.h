#ifndef SHADER_H
#define SHADER_H

#include <GL/gl.h>
#include "clib.h"
#include "vector.h"
#include "gl_functions.h"
#include "shader_code.h"

static stdcall void add_shader(GLuint program, GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader_(type);
    glShaderSource_(shader, 1, &source, NULL);
    glCompileShader_(shader);
    glAttachShader_(program, shader);
}

static stdcall GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = glCreateProgram_();
    add_shader(program, GL_VERTEX_SHADER, vertex_source);
    add_shader(program, GL_FRAGMENT_SHADER, fragment_source);
    glLinkProgram_(program);
    return program;
}

static stdcall void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = glGetUniformLocation_(program, identifier);
    glUniform3fv_(location, 1, value);
}

static stdcall void uniform_matrix3(GLuint program, const char* identifier, const matrix3 value)
{
    GLint location = glGetUniformLocation_(program, identifier);
    glUniformMatrix3fv_(location, 1, GL_TRUE, (const GLfloat*)value);
}

#endif

