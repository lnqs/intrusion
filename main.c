#include <stdbool.h>
#include <pthread.h>
#include <GL/glew.h>
#include <SDL.h>
#include "shader_code.h"
#include "4klang.inh"

typedef float vector3[3];
typedef float matrix3[3][3]; // [row][col]

#include "keypoint.h"

static const int resolution_x = 800;
static const int resolution_y = 600;
static const bool fullscreen = false;
static const char* window_caption = "Planeshift";
static const int audio_channels = 2;

static const float window_ratio = (float)resolution_x / resolution_y;

static int current_key_point = 1;

static vector3 position;
static matrix3 orientation;
static float box_scale;
static float box_radius;
static float sphere_radius;

static SAMPLE_TYPE sample_buffer[MAX_SAMPLES * audio_channels];
static int sample_position = 0;

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

static void sound_callback(void* userdata, Uint8* stream, int length)
{
    if (sample_position >= MAX_SAMPLES)
    {
        return;
    }

    static float* sample = sample_buffer;
    int16_t* p = (int16_t*)stream;

    for (int i = length / (sizeof(short) * audio_channels); i > 0 ; i--)
    {
        *p++ = (int16_t)(INT16_MAX * *sample++);
        *p++ = (int16_t)(INT16_MAX * *sample++);

        sample_position += 1;
    }
}

static void initialize_sound()
{
    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = audio_channels;
    spec.silence = 0;
    spec.samples = 4096;
    spec.size = 0;
    spec.callback = sound_callback;

    SDL_OpenAudio(&spec, NULL);
}

static void play_sound()
{
    pthread_t render_thread;
    pthread_create(&render_thread, NULL, __4klang_render, &sample_buffer);
    SDL_PauseAudio(0);
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

static void add_shader(GLuint program, GLenum type, const GLchar* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
}

static GLuint compile_program(const char* vertex_source, const char* fragment_source)
{
    GLuint program = glCreateProgram();
    add_shader(program, GL_VERTEX_SHADER, vertex_source);
    add_shader(program, GL_FRAGMENT_SHADER, fragment_source);
    glLinkProgram(program);
    return program;
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
        uniform_vector3(program, "fractal_params", (vector3){box_scale, box_radius, sphere_radius});

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

