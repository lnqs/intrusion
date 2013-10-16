#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include "linker.h"
#include <SDL.h>

#define sdl_call(name, type) ((type)sdl_functions[name ## _i])

#define SDL_GL_GetProcAddress_fn \
    sdl_call(SDL_GL_GetProcAddress, void* SDLCALL (*)(const char*))

#define SDL_SetVideoMode_fn \
    sdl_call(SDL_SetVideoMode, SDL_Surface* SDLCALL (*)(int, int, int, Uint32))

#define SDL_WM_SetCaption_fn \
    sdl_call(SDL_WM_SetCaption, void SDLCALL (*)(const char*, const char*))

#define SDL_ShowCursor_fn \
    sdl_call(SDL_ShowCursor, int SDLCALL (*)(int))

#define SDL_QuitSubSystem_fn \
    sdl_call(SDL_QuitSubSystem, void SDLCALL (*)(Uint32))

#define SDL_PollEvent_fn \
    sdl_call(SDL_PollEvent, int SDLCALL (*)(SDL_Event*))

#define SDL_GetTicks_fn \
    sdl_call(SDL_GetTicks, Uint32 SDLCALL (*)())

#define SDL_GL_SwapBuffers_fn \
    sdl_call(SDL_GL_SwapBuffers, void SDLCALL (*)())

#define SDL_OpenAudio_fn \
    sdl_call(SDL_OpenAudio, int SDLCALL (*)(SDL_AudioSpec*, SDL_AudioSpec*))

#define SDL_PauseAudio_fn \
    sdl_call(SDL_PauseAudio, void SDLCALL (*)(int))


// TODO: the versioning sucks.
static const char* sdl_library = "libSDL-1.2.so";

enum {
    SDL_GL_GetProcAddress_i,
    SDL_SetVideoMode_i,
    SDL_WM_SetCaption_i,
    SDL_ShowCursor_i,
    SDL_QuitSubSystem_i,
    SDL_PollEvent_i,
    SDL_GetTicks_i,
    SDL_GL_SwapBuffers_i,
    SDL_OpenAudio_i,
    SDL_PauseAudio_i
};

static const uint32_t sdl_hashes[] = {
    0x70a9a253, // SDL_GL_GetProcAddress
    0x7cc5e50f, // SDL_SetVideoMode
    0x5a36c844, // SDL_WM_SetCaption
    0xdcc5fcc6, // SDL_ShowCursor
    0xccc87339, // SDL_QuitSubSystem
    0x92f25140, // SDL_PollEvent
    0x0b38f265, // SDL_GetTicks
    0x811eb401, // SDL_GL_SwapBuffers
    0xf44169cb, // SDL_OpenAudio
    0x42850c57  // SDL_PauseAudio
};

static void* sdl_functions[sizeof(sdl_hashes) / sizeof(uint32_t)];

static stdcall void initialize_sdl_functions()
{
    load_library(sdl_library);

    for (int i = 0; i < sizeof(sdl_hashes) / sizeof(uint32_t); i++)
    {
        sdl_functions[i] = resolve_symbol(sdl_library, sdl_hashes[i]);
    }
}

#endif

