#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include "shader.h"
#include "vector_math.h"
#include "shader_code.h"
#include "sound.h"

static const int resolution_x = 640;
static const int resolution_y = 480;
static bool fullscreen = false;
static char* window_caption = "Planeshift";
static const float movement_speed = 0.5;
static const float mouse_speed = 0.5;
static const float scale_speed = 0.001;

static const float window_ratio = (float)resolution_x / resolution_y;

struct keypoint
{
    Uint32 time;
    vector3 position;
    matrix3 orientation;
    float box_scale;
    float box_radius;
    float sphere_radius;
};

static const struct keypoint keypoints[] = {
    {
        .time = 0,
        .position = { 0.0, 0.0, 4.3 },
        .orientation = MATRIX3_IDENTITY,
        .box_scale = -5.0,
        .box_radius = 3.0,
        .sphere_radius = 3.0,

    },
    {
        .time = 8000,
        .position = { 0.0, -8.0, 4.3 },
        .orientation = {
            { 1.08, -0.08,  0.00 },
            { 0.05,  0.58, -0.80 },
            { 0.07,  0.80,  0.60 }

        },
        .box_scale = -5.0,
        .box_radius = 3.0,
        .sphere_radius = 3.0
    },
    {
        .time = 20000,
        .position = { 0.0, -5.0, 7.5 },
        .orientation = {
            {  0.96, -0.04,  0.26 },
            {  0.24,  0.54, -0.80 },
            { -0.11,  0.83,  0.53 },
        },
        .box_scale = -5.0,
        .box_radius = 4.0,
        .sphere_radius = 3.0
    },
    {
        .time = 36000,
        .position = { 5.09, -1.66, 5.23 },
        .orientation = {
            { -0.03,  0.08, -0.99 },
            {  0.35, -0.92, -0.09 },
            { -0.93, -0.35,  0.00 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = 0.75
    },
    {
        .time = 46000,
        .position = { 5.06, 3.74, 5.29 },
        .orientation = {
            { -0.30,  0.93, -0.08 },
            {  0.04, -0.06, -0.98 },
            { -0.94, -0.31, -0.01 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = 0.75
    },
    {
        .time = 55000,
        .position = { 5.08, 8.65, 5.31 },
        .orientation = {
            { -0.02,  0.85, 0.47 },
            { -0.69, -0.34, 0.60 },
            {  0.71, -0.32, 0.60 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = 0.75
    },
    {
        .time = 70000,
        .position = { 0.33, 7.38, -1.03 },
        .orientation = {
            { 0.54,  0.76,  0.23 },
            { 0.17, -0.38,  0.86 },
            { 0.77, -0.44, -0.37 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = -2.0
    },
    {
        .time = 80000,
        .position = { -0.24, 5.19, -0.57 },
        .orientation = {
            { -0.04 , 0.90, -0.31 },
            {  0.93, -0.02, -0.19 },
            { -0.21, -0.31, -0.88 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = -2.0
    },
    {
        .time = 90000,
        .position = { -0.14, 3.67, 0.15 },
        .orientation = {
            { -0.04,  0.94, -0.05 },
            {  0.11, -0.04, -0.93 },
            { -0.94, -0.04, -0.09 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = -2.0
    },
    {
        .time = 100000,
        .position = { -0.25, 3.33, -0.70 },
        .orientation = {
            {  0.01,  0.93,  0.17 },
            { -0.94,  0.02, -0.05 },
            { -0.03, -0.17,  0.93 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = -2.0
    },
    {
        .time = 110000,
        .position = { -0.40, 3.62, -2.33 },
        .orientation = {
            { -0.05,  0.91,  0.24 },
            { -0.73,  0.11, -0.59 },
            { -0.58, -0.21,  0.71 }
        },
        .box_scale = -2.5,
        .box_radius = 3.0,
        .sphere_radius = -2.0
    },
    {
        .time = 120000,
        .position = { -0.69, 3.02, -3.72 },
        .orientation = {
            { -0.14,  0.65, -0.67 },
            {  0.18, -0.64, -0.67 },
            { -0.92, -0.21, -0.01 }
        },
        .box_scale = -2.5,
        .box_radius = 2.0,
        .sphere_radius = -2.0
    },
    {
        .time = 150000,
        .position = { 10.34, 3.07, -4.24 },
        .orientation = {
            {  0.05, -0.20, -0.91 },
            {  0.08, -0.91,  0.20 },
            { -0.93, -0.07, -0.04 }
        },
        .box_scale = -3.0,
        .box_radius = 5.0,
        .sphere_radius = -2.0
    }
};

static int current_key_point = 1;

static vector3 position = { 0.0, 0.0, 2.5 };
static matrix3 orientation = MATRIX3_IDENTITY;
static float box_scale = -2.5;
static float box_radius = 3.0;
static float sphere_radius = 3.0;

static void initialize_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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

static void move(const vector3 axis, float ticks_elapsed)
{
    vector3 movement;
    vector3_copy(movement, axis);
    vector3_multiply(movement, ticks_elapsed / 1000.0 * movement_speed);
    vector3_rotate(movement, orientation);
    vector3_add(position, movement);
}

static bool handle_keyboard(Uint32 ticks_elapsed)
{
    Uint8* keystate = SDL_GetKeyState(NULL);

    if (keystate[SDLK_ESCAPE])
    {
        return false;
    }

    if (keystate[SDLK_w])
    {
        move((vector3)VECTOR3_NEGATIVE_UNIT_Z, ticks_elapsed);
    }

    if (keystate[SDLK_a])
    {
        move((vector3)VECTOR3_NEGATIVE_UNIT_X, ticks_elapsed);
    }

    if (keystate[SDLK_s])
    {
        move((vector3)VECTOR3_UNIT_Z, ticks_elapsed);
    }

    if (keystate[SDLK_d])
    {
        move((vector3)VECTOR3_UNIT_X, ticks_elapsed);
    }

    if (keystate[SDLK_r])
    {
        box_scale -= ticks_elapsed * scale_speed;
    }

    if (keystate[SDLK_t])
    {
        box_scale += ticks_elapsed * scale_speed;
    }

    if (keystate[SDLK_f])
    {
        box_radius -= ticks_elapsed * scale_speed;
    }

    if (keystate[SDLK_g])
    {
        box_radius += ticks_elapsed * scale_speed;
    }

    if (keystate[SDLK_c])
    {
        sphere_radius -= ticks_elapsed * scale_speed;
    }

    if (keystate[SDLK_v])
    {
        sphere_radius += ticks_elapsed * scale_speed;
    }

    if (keystate[SDLK_q])
    {
        printf("Position:      { %f, %f, %f }\n"
               "Orientation:   { %f, %f, %f }\n"
               "               { %f, %f, %f }\n"
               "               { %f, %f, %f }\n"
               "Box Scale:     %f\n"
               "Box Radius:    %f\n"
               "Sphere Radius: %f\n",
               position[0], position[1], position[2],
               orientation[0][0], orientation[0][1], orientation[0][2],
               orientation[1][0], orientation[1][1], orientation[1][2],
               orientation[2][0], orientation[2][1], orientation[2][2],
               box_scale, box_radius, sphere_radius);
    }

    return true;
}

static void handle_mouse(Uint32 ticks_elapsed)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    x -= resolution_x / 2;
    y -= resolution_y / 2;

    matrix3 rotation = MATRIX3_IDENTITY;
    matrix3_rotate(rotation, (vector3)VECTOR3_UNIT_X, -y * (ticks_elapsed / 1000.0) * mouse_speed);
    matrix3_rotate(rotation, (vector3)VECTOR3_UNIT_Y, -x * (ticks_elapsed / 1000.0) * mouse_speed);
    matrix3_multiply(orientation, rotation);

    SDL_WarpMouse(resolution_x / 2, resolution_y / 2);
}

static bool handle_input(Uint32 ticks_elapsed)
{
    SDL_PumpEvents();

    handle_mouse(ticks_elapsed);
    return handle_keyboard(ticks_elapsed);
}

static float linear_step(float start, float end, float position, float duration)
{
    return start + (end - start) * (position / duration);
}

static void update_scene()
{
    if (current_key_point >= sizeof(keypoints) / sizeof(struct keypoint))
    {
        return;
    }

    Uint32 time = SDL_GetTicks();

    const struct keypoint* origin = &keypoints[current_key_point - 1];
    const struct keypoint* destination = &keypoints[current_key_point];
    const Uint32 transition_position = time - origin->time;
    const Uint32 transition_time = destination->time - origin->time;

    if (time > destination->time)
    {
        current_key_point++;
        return;
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
}

static void mainloop(GLint program)
{
    glUseProgram(program);

    Uint32 ticks = SDL_GetTicks();
    Uint32 ticks_elapsed = 0;

    while (handle_input(ticks_elapsed))
    {
        update_scene();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uniform_vector3(program, "position", position);
        uniform_matrix3(program, "orientation", orientation);
        uniform_float(program, "box_scale", box_scale);
        uniform_float(program, "box_radius", box_radius);
        uniform_float(program, "sphere_radius", sphere_radius);

        glBegin(GL_QUADS);
        glVertex3f(-window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio, -1.0, 0.0);
        glVertex3f( window_ratio,  1.0, 0.0);
        glVertex3f(-window_ratio,  1.0, 0.0);
        glEnd();

        SDL_GL_SwapBuffers();

        ticks_elapsed = SDL_GetTicks() - ticks;
        ticks = SDL_GetTicks();;
    }
}

int main(int argc, char** argv)
{
    initialize_sdl();
    initialize_glew();
    initialize_sound();

    GLuint program = compile_program(vertex_glsl, fragment_glsl);
    setup_viewport();

    play_sound();
    mainloop(program);

    cleanup_sdl();

    return 0;
}

