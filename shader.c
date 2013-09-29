#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

static GLuint compile_shader(GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = glCreateProgram();
    GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    return program;
}

void uniform_float(GLuint program, const char* identifier, float value)
{
    GLint location = glGetUniformLocation(program, identifier);
    glUniform1f(location, value);
}

void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = glGetUniformLocation(program, identifier);
    glUniform3fv(location, 1, value);
}

void uniform_matrix3(GLuint program, const char* identifier, const matrix3 value)
{
    GLint location = glGetUniformLocation(program, identifier);
    glUniformMatrix3fv(location, 1, GL_TRUE, (const GLfloat*)value);
}

