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
#include "shader_code.h"

static const int resolution_x = 800;
static const int resolution_y = 600;
static const bool fullscreen = false;
static const float window_ratio = (float)resolution_x / resolution_y;
static struct scene_state scene_state;
static const struct keypoint* keypoint = keypoints;

static stdcall void initialize_sdl()
{
    SDL_SetVideoMode_fn(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
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
        return true;
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
    {
        return true;
    }

    return false;
}

static stdcall bool update_scene()
{
    Uint32 time = SDL_GetTicks_fn();
    const struct keypoint* next = keypoint + 1;

    if (time > keypoints[sizeof(keypoints) / sizeof(struct keypoint) - 1].time)
    {
        return false;
    }

    if (time > next->time)
    {
        keypoint += 1;
        next += 1;
    }

    const float time_factor =
        (float)(time - keypoint->time) / (next->time - keypoint->time);

    float* state = (float*)&scene_state;
    float* origin = (float*)&keypoint->state;
    float* destination = (float*)&next->state;

    for (int i = 0; i < sizeof(struct scene_state) / sizeof(float); i++)
    {
        state[i] = origin[i] + (destination[i] - origin[i]) * time_factor;
    }

    return true;
}

static stdcall void mainloop()
{
    GLuint program = compile_program(vertex_glsl, fragment_glsl);
    glUseProgram_fn(program);

    while (!exit_requested() && update_scene())
    {
        uniform_vector3(program, "x", scene_state.position);
        uniform_matrix3(program, "o", scene_state.orientation);
        // Since the three parameters follow each other in the struct,
        // we just thread them as vector. It looks the same in memory anyway.
        uniform_vector3(program, "f", (float*)&scene_state.box_scale);

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

