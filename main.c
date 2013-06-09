#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include "shader_code.h"

static const int resolution_x = 640;
static const int resolution_y = 480;
static bool fullscreen = false;
static char* window_caption = "Planeshift";

static const float window_ratio = (float)resolution_x / resolution_y;

static void initialize_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    SDL_WM_SetCaption(window_caption, NULL);
    SDL_ShowCursor(SDL_DISABLE);
}

static void initialize_glew()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(2);
    }
}

static void setup_viewport()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0.0, 0.0, resolution_x, resolution_y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-window_ratio, window_ratio, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

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

static GLuint compile_program()
{
    GLuint program = glCreateProgram();
    GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_glsl);
    GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_glsl);
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

static void mainloop(GLint program)
{
    glUseProgram(program);

    SDL_Event event;
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_QUADS);
        glVertex3f(-window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio,  1.0, 0.0);
        glVertex3f(-window_ratio,  1.0, 0.0);
        glEnd();

        SDL_GL_SwapBuffers();

        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN && event.key.keysym.sym != SDLK_ESCAPE);
}

int main(int argc, char** argv)
{
    initialize_sdl();
    initialize_glew();
    GLuint program = compile_program();
    setup_viewport();

    mainloop(program);

    return 0;
}

