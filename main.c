#include <stdbool.h>
#include <SDL.h>
#include <GL/gl.h>
#include "clib.h"
#include "sdl_functions.h"
#include "gl_functions.h"
#include "vector.h"
#include "shader.h"
#include "sound.h"
#include "keypoint.h"

static const int resolution_x = 800;
static const int resolution_y = 600;
static const bool fullscreen = false;
static const char* window_caption = "Planeshift";
static const float window_ratio = (float)resolution_x / resolution_y;

static const struct keypoint* keypoint = keypoints;

static vector3 position;
static matrix3 orientation;
static float box_scale;
static float box_radius;
static float sphere_radius;

static stdcall void initialize_sdl()
{
    SDL_SetVideoMode_fn(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    SDL_WM_SetCaption_fn(window_caption, NULL);
    SDL_ShowCursor_fn(SDL_DISABLE);
}

static stdcall void cleanup_sdl()
{
    // SDL_Quit crashes since main() is removed, but we need this call to reset
    // the screen resolution when running fullscreen
    SDL_QuitSubSystem_fn(SDL_INIT_VIDEO);
}

static stdcall void setup_viewport()
{
    glMatrixMode_fn(GL_PROJECTION);
    glOrtho_fn(-window_ratio, window_ratio, -1.0, 1.0, -1.0, 1.0);
}

static stdcall bool exit_requested()
{
    SDL_Event event;
    SDL_PollEvent_fn(&event);

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

static stdcall float linear_step(float start, float end, float position, float duration)
{
    return start + (end - start) * (position / duration);
}

static stdcall bool update_scene()
{
    Uint32 time = SDL_GetTicks_fn();

    if (time > keypoints[sizeof(keypoints) / sizeof(struct keypoint) - 1].time)
    {
        return false;
    }

    const struct keypoint* origin = keypoint;
    const struct keypoint* destination = keypoint + 1;
    const Uint32 transition_position = time - origin->time;
    const Uint32 transition_time = destination->time - origin->time;

    if (time > destination->time)
    {
        keypoint++;
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

static stdcall void mainloop()
{
    GLuint program = compile_program(vertex_glsl, fragment_glsl);
    glUseProgram_fn(program);

    while (exit_requested() && update_scene())
    {
        uniform_vector3(program, "x", position);
        uniform_matrix3(program, "o", orientation);
        uniform_vector3(program, "f", (vector3){box_scale, box_radius, sphere_radius});

        glBegin_fn(GL_QUADS);
        glVertex3f_fn(-window_ratio, -1.0, 0.0);
        glVertex3f_fn( window_ratio, -1.0, 0.0);
        glVertex3f_fn( window_ratio,  1.0, 0.0);
        glVertex3f_fn(-window_ratio,  1.0, 0.0);
        glEnd_fn();

        SDL_GL_SwapBuffers_fn();
    }
}

void _start()
{
    initialize_sdl_functions();
    initialize_sdl();
    initialize_gl_functions();
    initialize_sound();

    setup_viewport();

    play_sound();
    mainloop();

    cleanup_sdl();

    exit_(0);
}

