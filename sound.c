#include "sound.h"
#include <pthread.h>
#include <signal.h>
#include <SDL.h>
#include "4klang.inh"

static const int channels = 2;
static const Uint16 play_buffer_size = 4096;

static pthread_t render_thread;
static SAMPLE_TYPE buffer[MAX_SAMPLES * channels];
static int position = 0;

static void callback(void* userdata, Uint8* stream, int length)
{
    if (position >= MAX_SAMPLES)
    {
        return;
    }

    static float* sample = buffer;
    int16_t* p = (int16_t*)stream;

    for (int i = length / (sizeof(short) * channels); i > 0 ; i--)
    {
        *p++ = (int16_t)(INT16_MAX * *sample++);
        *p++ = (int16_t)(INT16_MAX * *sample++);

        position += 1;
    }
}

void initialize_sound()
{
    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = channels;
    spec.silence = 0;
    spec.samples = play_buffer_size;
    spec.size = 0;
    spec.callback = callback;
    spec.userdata = buffer;

    SDL_OpenAudio(&spec, NULL);
}

void play_sound()
{
    pthread_create(&render_thread, NULL, __4klang_render, &buffer);
    SDL_PauseAudio(0);
}

void stop_sound()
{
    SDL_PauseAudio(1);
    pthread_kill(render_thread, SIGKILL);
    pthread_join(render_thread, NULL);
}

