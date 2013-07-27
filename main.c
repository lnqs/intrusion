#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include "vector_math.h"
#include "shader_code.h"

static const int resolution_x = 640;
static const int resolution_y = 480;
static bool fullscreen = false;
static char* window_caption = "Planeshift";
static const float movement_speed = 0.5;

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

static void uniform_vector3(GLuint program, const char* identifier, const vector3 value)
{
    GLint location = glGetUniformLocation(program, identifier);

    if (location == -1)
    {
        fprintf(stderr, "Failed to get location of uniform '%s'\n", identifier);
        exit(2);
    }

    glUniform3fv(location, 1, value);
}

static void move(const vector3 axis, float ticks_elapsed, vector3 position)
{
    vector3 movement;
    vector3_copy(movement, axis);
    vector3_multiply(movement, ticks_elapsed / 1000.0 * movement_speed);
    vector3_add(position, movement);
}

static bool handle_input(Uint32 ticks_elapsed, vector3 position)
{
    SDL_PumpEvents();
    Uint8* keystate = SDL_GetKeyState(NULL);

    if (keystate[SDLK_ESCAPE])
    {
        return false;
    }

    if (keystate[SDLK_w])
    {
        move(VECTOR3_NEGATIVE_UNIT_Z, ticks_elapsed, position);
    }

    if (keystate[SDLK_a])
    {
        move(VECTOR3_NEGATIVE_UNIT_X, ticks_elapsed, position);
    }

    if (keystate[SDLK_s])
    {
        move(VECTOR3_UNIT_Z, ticks_elapsed, position);
    }

    if (keystate[SDLK_d])
    {
        move(VECTOR3_UNIT_X, ticks_elapsed, position);
    }

    return true;
}

static void mainloop(GLint program)
{
    vector3 position = { 0.0, 0.0, 2.5 };

    glUseProgram(program);

    Uint32 ticks = SDL_GetTicks();
    Uint32 ticks_elapsed = 0;

    while (handle_input(ticks_elapsed, position))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uniform_vector3(program, "position", position);

        glBegin(GL_QUADS);
        glVertex3f(-window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio,  1.0, 0.0);
        glVertex3f(-window_ratio,  1.0, 0.0);
        glEnd();

        SDL_GL_SwapBuffers();

        ticks_elapsed = SDL_GetTicks() - ticks;
        ticks = SDL_GetTicks();
    }
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

