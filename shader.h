#ifndef SHADER_H
#define SHADER_H

#include <GL/gl.h>
#include "clib.h"
#include "vector.h"
#include "gl_functions.h"

static stdcall void add_shader(GLuint program, GLenum type, const GLchar* source)
{
    GLuint shader = gl.glCreateShader(type);
    gl.glShaderSource(shader, 1, &source, NULL);
    gl.glCompileShader(shader);
    gl.glAttachShader(program, shader);
}

static stdcall GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = gl.glCreateProgram();
    add_shader(program, GL_VERTEX_SHADER, vertex_source);
    add_shader(program, GL_FRAGMENT_SHADER, fragment_source);
    gl.glLinkProgram(program);
    return program;
}

static stdcall void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = gl.glGetUniformLocation(program, identifier);
    gl.glUniform3fv(location, 1, value);
}

static stdcall void uniform_matrix3(GLuint program, const char* identifier, const matrix3 value)
{
    GLint location = gl.glGetUniformLocation(program, identifier);
    gl.glUniformMatrix3fv(location, 1, GL_TRUE, (const GLfloat*)value);
}

#endif

