#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include "shader.h"
#include "vector_math.h"
#include "shader_code.h"

static const int resolution_x = 640;
static const int resolution_y = 480;
static bool fullscreen = false;
static char* window_caption = "Planeshift";
static const float movement_speed = 0.5;
static const float mouse_speed = 0.5;

static const float window_ratio = (float)resolution_x / resolution_y;

static void initialize_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(resolution_x, resolution_y, 0,
            SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
    SDL_WM_SetCaption(window_caption, NULL);
    SDL_ShowCursor(SDL_DISABLE);
}

static void initialize_glew()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(2);
    }
}

static void setup_viewport()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0.0, 0.0, resolution_x, resolution_y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-window_ratio, window_ratio, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void move(const vector3 axis, float ticks_elapsed, vector3 position, matrix3 orientation)
{
    vector3 movement;
    vector3_copy(movement, axis);
    vector3_multiply(movement, ticks_elapsed / 1000.0 * movement_speed);
    vector3_rotate(movement, orientation);
    vector3_add(position, movement);
}

static bool handle_keyboard(Uint32 ticks_elapsed, vector3 position, matrix3 orientation)
{
    Uint8* keystate = SDL_GetKeyState(NULL);

    if (keystate[SDLK_ESCAPE])
    {
        return false;
    }

    if (keystate[SDLK_w])
    {
        move(VECTOR3_NEGATIVE_UNIT_Z, ticks_elapsed, position, orientation);
    }

    if (keystate[SDLK_a])
    {
        move(VECTOR3_NEGATIVE_UNIT_X, ticks_elapsed, position, orientation);
    }

    if (keystate[SDLK_s])
    {
        move(VECTOR3_UNIT_Z, ticks_elapsed, position, orientation);
    }

    if (keystate[SDLK_d])
    {
        move(VECTOR3_UNIT_X, ticks_elapsed, position, orientation);
    }

    return true;
}

static void handle_mouse(Uint32 ticks_elapsed, matrix3 orientation)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    x -= resolution_x / 2;
    y -= resolution_y / 2;

    matrix3 rotation = MATRIX3_IDENTITY;
    matrix3_rotate(rotation, VECTOR3_UNIT_X, -y * (ticks_elapsed / 1000.0) * mouse_speed);
    matrix3_rotate(rotation, VECTOR3_UNIT_Y, -x * (ticks_elapsed / 1000.0) * mouse_speed);
    matrix3_multiply(orientation, rotation);

    SDL_WarpMouse(resolution_x / 2, resolution_y / 2);
}

static bool handle_input(Uint32 ticks_elapsed, vector3 position, matrix3 orientation)
{
    SDL_PumpEvents();

    handle_mouse(ticks_elapsed, orientation);
    return handle_keyboard(ticks_elapsed, position, orientation);
}

static void mainloop(GLint program)
{
    vector3 position = { 0.0, 0.0, 2.5 };
    matrix3 orientation = MATRIX3_IDENTITY;

    glUseProgram(program);

    Uint32 ticks = SDL_GetTicks();
    Uint32 ticks_elapsed = 0;

    while (handle_input(ticks_elapsed, position, orientation))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uniform_vector3(program, "position", position);
        uniform_matrix3(program, "orientation", orientation);

        glBegin(GL_QUADS);
        glVertex3f(-window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio,  1.0, 0.0);
        glVertex3f(-window_ratio,  1.0, 0.0);
        glEnd();

        SDL_GL_SwapBuffers();

        ticks_elapsed = SDL_GetTicks() - ticks;
        ticks = SDL_GetTicks();
    }
}

int main(int argc, char** argv)
{
    initialize_sdl();
    initialize_glew();
    GLuint program = compile_program(vertex_glsl, fragment_glsl);
    setup_viewport();

    mainloop(program);

    return 0;
}

