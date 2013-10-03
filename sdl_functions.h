#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include <dlfcn.h>
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

static const char* sdl_symbols[] = {
    "SDL_GL_GetProcAddress",
    "SDL_SetVideoMode",
    "SDL_WM_SetCaption",
    "SDL_ShowCursor",
    "SDL_QuitSubSystem",
    "SDL_PollEvent",
    "SDL_GetTicks",
    "SDL_GL_SwapBuffers",
    "SDL_OpenAudio",
    "SDL_PauseAudio"
};

static const void* sdl_functions[sizeof(sdl_symbols) / sizeof(const char*)];

static stdcall void initialize_sdl_functions()
{
    void* libsdl = dlopen("libSDL-1.2.so", RTLD_NOW);

    for (int i = 0; i < sizeof(sdl_symbols) / sizeof(const char*); i++)
    {
        sdl_functions[i] = dlsym(libsdl, sdl_symbols[i]);
    }
}

#endif

