#include <stdbool.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <GL/glew.h>
#include <SDL.h>
#include "shader_code.h"
#include "4klang.inh"

static const int resolution_x = 800;
static const int resolution_y = 600;
static const bool fullscreen = false;
static const char* window_caption = "Planeshift";
static const float window_ratio = (float)resolution_x / resolution_y;

static const int sound_channels = 2;
static const size_t sound_thread_stack_size = 1024 * 1024;

typedef float vector3[3];
typedef float matrix3[3][3]; // [row][col]

struct keypoint
{
    Uint32 time;
    vector3 position;
    matrix3 orientation;
    float box_scale;
    float box_radius;
    float sphere_radius;
} __attribute__((__packed__));

#include "keypoint.h"

static int keypoint = 1;

static vector3 position;
static matrix3 orientation;
static float box_scale;
static float box_radius;
static float sphere_radius;

static unsigned char sound_buffer[MAX_SAMPLES * sizeof(SAMPLE_TYPE) * sound_channels];
static int sound_buffer_position = 0;
static unsigned char sound_thread_stack[sound_thread_stack_size];

static void initialize_sdl()
{
    SDL_SetVideoMode(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    SDL_WM_SetCaption(window_caption, NULL);
    SDL_ShowCursor(SDL_DISABLE);
}

static void cleanup_sdl()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

static void initialize_glew()
{
    glewExperimental = GL_TRUE;
    glewInit();
}

static void memcpy_(void* dest, const void* src, size_t n)
{
    __asm__ volatile ("cld\n"
                      "rep\n"
                      "movsl\n"
                      "movl %3,%2\n"
                      "rep\n"
                      "movsb\n"
                      :
                      : "S" (src),
                        "D" (dest),
                        "c" (n / 4),
                        "r" (n % 4));
}

static void sound_callback(void* userdata, Uint8* stream, int length)
{
    memcpy_(stream, sound_buffer + sound_buffer_position, length);
    sound_buffer_position += length;
}

static void initialize_sound()
{
    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = sound_channels;
    spec.silence = 0;
    spec.samples = 4096;
    spec.size = 0;
    spec.callback = sound_callback;

    SDL_OpenAudio(&spec, NULL);
}

static void exit_(int code)
{
    __asm__ volatile ("int $0x80"
                      :
                      : "a" (SYS_exit_group),
                        "b" (code));
}

static void clone_(int (*fn)(void*), void* stack, int flags, void* data)
{
    __asm__ volatile ("subl $4,%2\n"
                      "movl %4,(%2)\n"
                      "int $0x80\n"
                      "testl %0,%0\n"
                      "jne 1f\n"
                      "call *%3\n"
                      "movl %5,%0\n"
                      "int $0x80\n"
                      "1:\n"
                      :
                      : "a" (SYS_clone),
                        "b" (flags),
                        "c" (stack),
                        "r" (fn),
                        "r" (data),
                        "r" (SYS_exit));
}

static void play_sound()
{
    clone_((void*)__4klang_render,
            sound_thread_stack + sizeof(sound_thread_stack),
            CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_SYSVSEM,
            sound_buffer);
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
    if (keypoint >= sizeof(keypoints) / sizeof(struct keypoint))
    {
        return false;
    }

    Uint32 time = SDL_GetTicks();

    const struct keypoint* origin = &keypoints[keypoint - 1];
    const struct keypoint* destination = &keypoints[keypoint];
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

static void mainloop()
{
    GLuint program = compile_program(vertex_glsl, fragment_glsl);
    glUseProgram(program);

    while (exit_requested() && update_scene())
    {
        uniform_vector3(program, "x", position);
        uniform_matrix3(program, "o", orientation);
        uniform_vector3(program, "f", (vector3){box_scale, box_radius, sphere_radius});

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

    setup_viewport();

    play_sound();
    mainloop();

    cleanup_sdl();

    exit_(0);
}

