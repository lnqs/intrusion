#include <stdbool.h>
#include <GL/gl.h>
#include <SDL.h>

static const int resolution_x = 640;
static const int resolution_y = 480;
static bool fullscreen = false;
static char* window_caption = "Planeshift";

static void initialize()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetVideoMode(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    SDL_WM_SetCaption(window_caption, NULL);
    SDL_ShowCursor(SDL_DISABLE);
}

static void cleanup()
{
    SDL_Quit();
}

static void mainloop()
{
    SDL_Event event;
    do
    {
        SDL_GL_SwapBuffers();

        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN && event.key.keysym.sym != SDLK_ESCAPE);
}

int main(int argc, char** argv)
{
    initialize();
    mainloop();
    cleanup();
    return 0;
}

