#include <stdbool.h>
#include <SDL.h>
#include <GL/gl.h>
#include "defines.h"
#include "shader_defines.h"
#include "clib.h"
#include "scene_state.h"
#include "sdl_functions.h"
#include "gl_functions.h"
#include "vector.h"
#include "shader.h"
#include "textrender.h"
#include "sound.h"
#include "keypoint.h"
#include "shader_code.h"

// While we only one float here, we make it three continous one to be able to
// pass it as vec3 to the shader.
// This way, we don't need to import another glUniform*-call.
struct effect_parameters
{
    float skew_multiplier;
    float ignore[2];
} packed;

static struct scene_state scene_state;
static const struct keypoint* keypoint = keypoint_points;
static struct effect_parameters effect_parameters;

static GLuint overlay_texture;

static stdcall void setup_window()
{
    sdl_functions.SDL_SetVideoMode(RESOLUTION_X, RESOLUTION_Y, 0,
            SDL_OPENGL | (FULLSCREEN ? SDL_FULLSCREEN : 0));
    sdl_functions.SDL_ShowCursor(SDL_DISABLE);
}

static stdcall void cleanup()
{
    // SDL_Quit crashes since main() is removed, but we need this call to reset
    // the screen resolution when running fullscreen
    sdl_functions.SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

static stdcall void create_overlay_texture(GLuint program)
{
    gl_functions.glActiveTexture(GL_TEXTURE0);
    gl_functions.glBindTexture(GL_TEXTURE_2D, overlay_texture);
    gl_functions.glGenTextures(1, &overlay_texture);

    shader_uniform_int(program, uniform(uf_text_texture), 0);

    gl_functions.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl_functions.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

static stdcall bool exit_requested()
{
    SDL_Event event;
    sdl_functions.SDL_PollEvent(&event);

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
    // Transition to next keypoint is handled here.
    // This function also adds a short 'skew' every time a keypoint is reached.
    // Handling it this 'magic' way sucks, it would be nicer to have it as part
    // of the keypoint-definition, but since it's only a short effect, we would
    // a new mechanism to read them from the keypoints, or a lot of additionals
    // points, both leading to way too much code.

    uint32_t time = sdl_functions.SDL_GetTicks();
    const struct keypoint* next = keypoint + 1;

    if (time > keypoint_points[sizeof(keypoint_points) / sizeof(struct keypoint) - 1].time)
    {
        return false;
    }

    if (time > next->time)
    {
        effect_parameters.skew_multiplier = MAX_SKEW;

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

    effect_parameters.skew_multiplier /= (time - keypoint->time) * SKEW_DECREASING_MULTIPLIER;

    return true;
}

static stdcall void mainloop(GLuint program)
{
    GLuint position_location = gl_functions.glGetAttribLocation(program, uniform(in_position));
    GLuint texcoord_location = gl_functions.glGetAttribLocation(program, uniform(in_texcoord));

    while (!exit_requested() && update_scene())
    {
        shader_uniform_vector3(program, uniform(uf_cam_position), scene_state.position);
        shader_uniform_matrix3(program, uniform(uf_cam_orientation), scene_state.orientation);
        // Since the three parameters follow each other in the struct,
        // we just treat them as vector to save some bytes.
        // It looks the same in memory anyway.
        shader_uniform_vector3(program, uniform(uf_fractal_params), (float*)&scene_state.box_scale);
        shader_uniform_vector3(program, uniform(uf_effect_params), (float*)&effect_parameters);

        gl_functions.glBegin(GL_QUADS);
        gl_functions.glVertexAttrib2f(position_location, -WINDOW_RATIO, -1.0);
        gl_functions.glVertexAttrib2f(texcoord_location, 1.0, 1.0);
        gl_functions.glVertexAttrib2f(position_location, WINDOW_RATIO, -1.0);
        gl_functions.glVertexAttrib2f(texcoord_location, 1.0, 0.0);
        gl_functions.glVertexAttrib2f(position_location, WINDOW_RATIO, 1.0);
        gl_functions.glVertexAttrib2f(texcoord_location, 0.0, 0.0);
        gl_functions.glVertexAttrib2f(position_location, -WINDOW_RATIO, 1.0);
        gl_functions.glVertexAttrib2f(texcoord_location, 0.0, 1.0);
        gl_functions.glEnd();

        gl_functions.glUseProgram(program);

        sdl_functions.SDL_GL_SwapBuffers();
    }
}

// Save overhead from crt1.o, get control from entry point on
void _start()
{
    sdl_functions_initialize();
    setup_window();
    gl_functions_initialize();
    sound_initialize();

    GLuint program = shader_compile_program(vertex_glsl, fragment_glsl);
    create_overlay_texture(program);

    /////// TODO: Testcode, remove
    textrender_set_text(_("int main(int argc, char** argv)\n"
                         "{\n"
                         "    printf(\"Hello World!\");\n"
                         "}"));
    gl_functions.glActiveTexture(GL_TEXTURE0);
    gl_functions.glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
            OVERLAY_TEXTURE_WIDTH, OVERLAY_TEXTURE_HEIGHT,
            0, GL_RED, GL_UNSIGNED_BYTE, textrender_buffer);
    ////////////

    sound_play();
    mainloop(program);

    cleanup();

    clib_exit(0);
}

