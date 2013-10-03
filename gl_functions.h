#ifndef GL_FUNCTIONS_H
#define GL_FUNCTIONS_H

#include <GL/gl.h>
#include <SDL.h>

static GLAPI void GLAPIENTRY (*glMatrixMode_)(GLenum);
static GLAPI void GLAPIENTRY (*glOrtho_)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);

static GLAPI void GLAPIENTRY (*glBegin_)(GLenum);
static GLAPI void GLAPIENTRY (*glEnd_)();
static GLAPI void GLAPIENTRY (*glVertex3f_)(GLfloat, GLfloat, GLfloat);

static GLAPI GLuint GLAPIENTRY (*glCreateShader_)(GLenum);
static GLAPI void GLAPIENTRY (*glShaderSource_)(GLuint, GLsizei count, const GLchar**, const GLint*);
static GLAPI void GLAPIENTRY (*glCompileShader_)(GLuint);
static GLAPI void GLAPIENTRY (*glAttachShader_)(GLuint, GLuint);
static GLAPI GLuint GLAPIENTRY (*glCreateProgram_)();
static GLAPI void GLAPIENTRY (*glLinkProgram_)(GLuint);
static GLAPI void GLAPIENTRY (*glUseProgram_)(GLuint);

static GLAPI GLint GLAPIENTRY (*glGetUniformLocation_)(GLuint, const GLchar*);
static GLAPI void GLAPIENTRY (*glUniform3fv_)(GLint, GLsizei, const GLfloat*);
static GLAPI void GLAPIENTRY (*glUniformMatrix3fv_)(GLint, GLsizei, GLboolean, const GLfloat*);

static void initialize_gl_functions()
{
    glMatrixMode_ = SDL_GL_GetProcAddress("glMatrixMode");
    glOrtho_ = SDL_GL_GetProcAddress("glOrtho");

    glBegin_ = SDL_GL_GetProcAddress("glBegin");
    glEnd_ = SDL_GL_GetProcAddress("glEnd");
    glVertex3f_ = SDL_GL_GetProcAddress("glVertex3f");

    glCreateShader_ = SDL_GL_GetProcAddress("glCreateShader");
    glShaderSource_ = SDL_GL_GetProcAddress("glShaderSource");
    glCompileShader_ = SDL_GL_GetProcAddress("glCompileShader");
    glAttachShader_ = SDL_GL_GetProcAddress("glAttachShader");
    glCreateProgram_ = SDL_GL_GetProcAddress("glCreateProgram");
    glLinkProgram_ = SDL_GL_GetProcAddress("glLinkProgram");
    glUseProgram_ = SDL_GL_GetProcAddress("glUseProgram");

    glGetUniformLocation_ = SDL_GL_GetProcAddress("glGetUniformLocation");
    glUniform3fv_ = SDL_GL_GetProcAddress("glUniform3fv");
    glUniformMatrix3fv_ = SDL_GL_GetProcAddress("glUniformMatrix3fv");
}

#endif

