#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include "vector_math.h"
#include "shader_code.h"
#include "sound.h"
#include "keypoint.h"

static const int resolution_x = 800;
static const int resolution_y = 600;
static bool fullscreen = false;
static char* window_caption = "Planeshift";

static const float window_ratio = (float)resolution_x / resolution_y;

static int current_key_point = 1;

static vector3 position;
static matrix3 orientation;
static float box_scale;
static float box_radius;
static float sphere_radius;

static void initialize_sdl()
{
    SDL_SetVideoMode(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    SDL_WM_SetCaption(window_caption, NULL);
    SDL_ShowCursor(SDL_DISABLE);
}

static void cleanup_sdl()
{
    SDL_Quit();
}

static void initialize_glew()
{
    glewExperimental = GL_TRUE;
    glewInit();
}

static void setup_viewport()
{
    glMatrixMode(GL_PROJECTION);
    glOrtho(-window_ratio, window_ratio, -1.0, 1.0, -1.0, 1.0);
}

static bool exit_requested()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
        return false;
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
    {
        return false;
    }

    return true;
}

static GLuint compile_shader(GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

static GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = glCreateProgram();
    GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    return program;
}

static void uniform_float(GLuint program, const char* identifier, float value)
{
    GLint location = glGetUniformLocation(program, identifier);
    glUniform1f(location, value);
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

static float linear_step(float start, float end, float position, float duration)
{
    return start + (end - start) * (position / duration);
}

static bool update_scene()
{
    if (current_key_point >= sizeof(keypoints) / sizeof(struct keypoint))
    {
        return false;
    }

    Uint32 time = SDL_GetTicks();

    const struct keypoint* origin = &keypoints[current_key_point - 1];
    const struct keypoint* destination = &keypoints[current_key_point];
    const Uint32 transition_position = time - origin->time;
    const Uint32 transition_time = destination->time - origin->time;

    if (time > destination->time)
    {
        current_key_point++;
        return true;
    }

    for (size_t i = 0; i < 3; i++)
    {
        position[i] = linear_step(origin->position[i],
                destination->position[i], transition_position, transition_time);
    }

    for (size_t row = 0; row < 3; row++)
    {
        for (size_t col = 0; col < 3; col++)
        {
            orientation[row][col] = linear_step(
                    origin->orientation[row][col],
                    destination->orientation[row][col],
                    transition_position, transition_time);
        }
    }

    box_scale = linear_step(origin->box_scale,
            destination->box_scale, transition_position, transition_time);
    box_radius = linear_step(origin->box_radius,
            destination->box_radius, transition_position, transition_time);
    sphere_radius = linear_step(origin->sphere_radius,
            destination->sphere_radius, transition_position, transition_time);

    return true;
}

static void mainloop(GLint program)
{
    glUseProgram(program);

    while (exit_requested() && update_scene())
    {
        uniform_vector3(program, "position", position);
        uniform_matrix3(program, "orientation", orientation);
        uniform_float(program, "box_scale", box_scale);
        uniform_float(program, "box_radius", box_radius);
        uniform_float(program, "sphere_radius", sphere_radius);

        glBegin(GL_QUADS);
        glVertex3f(-window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio,  1.0, 0.0);
        glVertex3f(-window_ratio,  1.0, 0.0);
        glEnd();

        SDL_GL_SwapBuffers();
    }
}

void _start()
{
    initialize_sdl();
    initialize_glew();
    initialize_sound();

    GLuint program = compile_program(vertex_glsl, fragment_glsl);
    setup_viewport();

    play_sound();
    mainloop(program);

    cleanup_sdl();

    exit(0);
}

