#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

static GLuint compile_shader(GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        char log[log_length];
        glGetShaderInfoLog(shader, log_length, NULL, log);
        fprintf(stderr, "Failed to compile shader: %s\n", log);
        exit(2);
    }

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

    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        int log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        char log[log_length];
        glGetProgramInfoLog(program, log_length, NULL, log);
        fprintf(stderr, "Failed to link program: %s\n", log);
        exit(2);
    }

    return program;
}

void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = glGetUniformLocation(program, identifier);

    if (location == -1)
    {
        fprintf(stderr, "Failed to get location of uniform '%s'\n", identifier);
        exit(2);
    }

    glUniform3fv(location, 1, value);
}

