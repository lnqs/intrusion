#ifndef SHADER_H
#define SHADER_H

#include <GL/gl.h>
#include "clib.h"
#include "vector.h"
#include "gl_functions.h"

static stdcall void add_shader(GLuint program, GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader_fn(type);
    glShaderSource_fn(shader, 1, &source, NULL);
    glCompileShader_fn(shader);
    glAttachShader_fn(program, shader);
}

static stdcall GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = glCreateProgram_fn();
    add_shader(program, GL_VERTEX_SHADER, vertex_source);
    add_shader(program, GL_FRAGMENT_SHADER, fragment_source);
    glLinkProgram_fn(program);
    return program;
}

static stdcall void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = glGetUniformLocation_fn(program, identifier);
    glUniform3fv_fn(location, 1, value);
}

static stdcall void uniform_matrix3(GLuint program, const char* identifier, const matrix3 value)
{
    GLint location = glGetUniformLocation_fn(program, identifier);
    glUniformMatrix3fv_fn(location, 1, GL_TRUE, (const GLfloat*)value);
}

#endif

