#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include "vector.h"
#include "shader_code.h"

static void add_shader(GLuint program, GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
}

static GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = glCreateProgram();
    add_shader(program, GL_VERTEX_SHADER, vertex_source);
    add_shader(program, GL_FRAGMENT_SHADER, fragment_source);
    glLinkProgram(program);
    return program;
}

static void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = glGetUniformLocation(program, identifier);
    glUniform3fv(location, 1, value);
}

static void uniform_matrix3(GLuint program, const char* identifier, const matrix3 value)
{
    GLint location = glGetUniformLocation(program, identifier);
    glUniformMatrix3fv(location, 1, GL_TRUE, (const GLfloat*)value);
}

#endif

