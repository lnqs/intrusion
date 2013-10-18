#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include <SDL.h>
#include "linker.h"

static const char* sdl_library = "libSDL-1.2.so";

// the members of this struct have to have the same order as in the hashes-array!
static struct {
    void* SDLCALL (*SDL_GL_GetProcAddress)(const char*);
    SDL_Surface* SDLCALL (*SDL_SetVideoMode)(int, int, int, Uint32);
    int SDLCALL (*SDL_ShowCursor)(int);
    void SDLCALL (*SDL_QuitSubSystem)(Uint32);
    int SDLCALL (*SDL_PollEvent)(SDL_Event*);
    Uint32 SDLCALL (*SDL_GetTicks)();
    void SDLCALL (*SDL_GL_SwapBuffers)();
    int SDLCALL (*SDL_OpenAudio)(SDL_AudioSpec*, SDL_AudioSpec*);
    void SDLCALL (*SDL_PauseAudio)(int);
} sdl;

static const uint32_t sdl_hashes[] = {
    0x70a9a253, // SDL_GL_GetProcAddress
    0x7cc5e50f, // SDL_SetVideoMode
    0xdcc5fcc6, // SDL_ShowCursor
    0xccc87339, // SDL_QuitSubSystem
    0x92f25140, // SDL_PollEvent
    0x0b38f265, // SDL_GetTicks
    0x811eb401, // SDL_GL_SwapBuffers
    0xf44169cb, // SDL_OpenAudio
    0x42850c57  // SDL_PauseAudio
};

static stdcall void initialize_sdl_functions()
{
    load_library(sdl_library);

    for (int i = 0; i < sizeof(sdl_hashes) / sizeof(uint32_t); i++)
    {
        ((void**)&sdl)[i] = resolve_symbol(sdl_library, sdl_hashes[i]);
    }
}

#endif

