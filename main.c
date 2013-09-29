#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include "shader.h"
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

static vector3 position = { 0.0, 0.0, 2.5 };
static matrix3 orientation = MATRIX3_IDENTITY;
static float box_scale = -2.5;
static float box_radius = 3.0;
static float sphere_radius = 3.0;

static void initialize_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);
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
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0.0, 0.0, resolution_x, resolution_y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-window_ratio, window_ratio, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    stop_sound();

    cleanup_sdl();

    exit(0);
}

