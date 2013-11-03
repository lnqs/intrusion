#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include "clib.h"
#include "sdl_functions.h"
#include "data/4klang.inh"

static const int sound_channels = 2;

static unsigned char sound_buffer[MAX_SAMPLES * sizeof(SAMPLE_TYPE) * sound_channels];
static unsigned char* sound_buffer_position = sound_buffer;
static unsigned char sound_thread_stack[1024 * 1024];

static void sound_callback(void* userdata, Uint8* stream, int length)
{
    clib_inaccurate_memcpy(stream, sound_buffer_position, length);
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

static stdcall void sound_initialize()
{
    sdl_functions.SDL_OpenAudio(&sound_spec, NULL);
}

static stdcall void sound_play()
{
    clib_clone((int (*)(void*))__4klang_render,
            sound_thread_stack + sizeof(sound_thread_stack),
            CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_SYSVSEM,
            sound_buffer);
    sdl_functions.SDL_PauseAudio(0);
}

static stdcall void sound_stop()
{
    sdl_functions.SDL_PauseAudio(1);
}

#endif

