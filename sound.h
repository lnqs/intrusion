#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include "clib.h"
#include "4klang.inh"

static const int sound_channels = 2;
static const size_t sound_thread_stack_size = 1024 * 1024;

static unsigned char sound_buffer[MAX_SAMPLES * sizeof(SAMPLE_TYPE) * sound_channels];
static int sound_buffer_position = 0;
static unsigned char sound_thread_stack[sound_thread_stack_size];

static void sound_callback(void* userdata, Uint8* stream, int length)
{
    inaccurate_memcpy(stream, sound_buffer + sound_buffer_position, length);
    sound_buffer_position += length;
}

static SDL_AudioSpec sound_spec = {
    .freq = SAMPLE_RATE,
    .format = AUDIO_S16SYS,
    .channels = sound_channels,
    .silence = 0,
    .samples = 4096,
    .size = 0,
    .callback = sound_callback
};

static void initialize_sound()
{
    SDL_OpenAudio(&sound_spec, NULL);
}

static void play_sound()
{
    clone_((void*)__4klang_render,
            sound_thread_stack + sizeof(sound_thread_stack),
            CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_SYSVSEM,
            sound_buffer);
    SDL_PauseAudio(0);
}

#endif

