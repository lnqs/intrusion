#ifndef KEYPOINT_H
#define KEYPOINT_H

#include "clib.h"
#include "scene_state.h"
#include "vector.h"

struct keypoint
{
    uint32_t time;
    struct scene_state state;
} packed;

struct keypoint_text
{
    uint32_t time;
    const char* text;
    enum console_text_type type;
} packed;

static const struct keypoint keypoint_points[] = {
    {
        .time = 0 * 1000,
        .state = {
            .position = { 0.00f, 0.00f, 4.30f },
            .orientation = {
                {  0.00f,  0.00f,  0.00f },
                {  0.00f,  0.00f,  0.00f },
                {  0.00f,  0.00f,  0.00f }
            },
            .box_scale = 0.0f,
            .box_radius = 0.0f,
            .sphere_radius = 0.0f,
        }
    },
    {
        .time = 3 * 1000,
        .state = {
            .position = { 0.00f, 0.00f, 4.30f },
            .orientation = {
                {  0.00f,  0.00f,  0.00f },
                {  0.00f,  0.00f,  0.00f },
                {  0.00f,  0.00f,  0.00f }
            },
            .box_scale = 0.0f,
            .box_radius = 0.0f,
            .sphere_radius = 0.0f,
        }
    },
    {
        .time = 13 * 1000,
        .state = {
            .position = { 0.23f, -5.74f, 7.96f },
            .orientation = {
                {  0.96f, -0.04f,  0.26f },
                {  0.24f,  0.54f, -0.80f },
                { -0.11f,  0.83f,  0.53f }
            },
            .box_scale = -5.0f,
            .box_radius = 4.0f,
            .sphere_radius = 3.0f
        }
    },
    {
        .time = 20 * 1000,
        .state = {
            .position = { 2.51f, -4.36f, 6.77f },
            .orientation = {
                {  0.60f, -0.00f, -0.17f },
                {  0.26f,  0.03f, -0.54f },
                { -0.39f,  0.41f,  0.34f }
            },
            .box_scale = -4.1f,
            .box_radius = 3.6f,
            .sphere_radius = 2.2f
        }
    },
    {
        .time = 30 * 1000,
        .state = {
            .position = { 5.09f, -1.66f, 5.23f },
            .orientation = {
                { -0.03f,  0.08f, -0.99f },
                {  0.35f, -0.92f, -0.09f },
                { -0.93f, -0.35f,  0.00f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = 0.75f
        }
    },
    {
        .time = 40 * 1000,
        .state = {
            .position = { 5.06f, 3.74f, 5.29f },
            .orientation = {
                { -0.30f,  0.93f, -0.08f },
                {  0.04f, -0.06f, -0.98f },
                { -0.94f, -0.31f, -0.01f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = 0.75f
        }
    },
    {
        .time = 50 * 1000,
        .state = {
            .position = { 5.08f, 8.65f, 5.31f },
            .orientation = {
                { -0.02f,  0.85f, 0.47f },
                { -0.69f, -0.34f, 0.60f },
                {  0.71f, -0.32f, 0.60f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = 0.75f
        }
    },
    {
        .time = 60 * 1000,
        .state = {
            .position = { 0.33f, 7.38f, -1.03f },
            .orientation = {
                { 0.54f,  0.76f,  0.23f },
                { 0.17f, -0.38f,  0.86f },
                { 0.77f, -0.44f, -0.37f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 70 * 1000,
        .state = {
            .position = { -0.24f, 5.19f, -0.57f },
            .orientation = {
                { -0.04f,  0.90f, -0.31f },
                {  0.93f, -0.02f, -0.19f },
                { -0.21f, -0.31f, -0.88f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 80 * 1000,
        .state = {
            .position = { -0.14f, 3.67f, 0.15f },
            .orientation = {
                { -0.04f,  0.94f, -0.05f },
                {  0.11f, -0.04f, -0.93f },
                { -0.94f, -0.04f, -0.09f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 90 * 1000,
        .state = {
            .position = { -0.25f, 3.33f, -0.70f },
            .orientation = {
                {  0.01f,  0.93f,  0.17f },
                { -0.94f,  0.02f, -0.05f },
                { -0.03f, -0.17f,  0.93f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 100 * 1000,
        .state = {
            .position = { -0.40f, 3.62f, -2.33f },
            .orientation = {
                { -0.05f,  0.91f,  0.24f },
                { -0.73f,  0.11f, -0.59f },
                { -0.58f, -0.21f,  0.71f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 110 * 1000,
        .state = {
            .position = { -0.69f, 3.02f, -3.72f },
            .orientation = {
                { -0.14f,  0.65f, -0.67f },
                {  0.18f, -0.64f, -0.67f },
                { -0.92f, -0.21f, -0.01f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 120 * 1000,
        .state = {
            .position = { -1.05f, 2.77f, -4.57f },
            .orientation = {
                {  0.50f,  0.79f, -0.05f },
                {  0.78f, -0.49f, -0.11f },
                { -0.15f,  0.02f, -0.92f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 130 * 1000,
        .state = {
            .position = { -1.33f, -0.22f, -4.94f },
            .orientation = {
                { -0.02f,  0.66f, -0.66f },
                {  0.93f, -0.01f, -0.06f },
                { -0.07f, -0.66f, -0.65f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 140 * 1000,
        .state = {
            .position = { -0.88f, -0.14f, -3.26f },
            .orientation = {
                { -0.01f,  0.89f,  0.28f },
                {  0.93f,  0.02f, -0.00f },
                { -0.03f,  0.28f, -0.89f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 150 * 1000,
        .state = {
            .position = { -2.48f, -0.08f, -1.67f },
            .orientation = {
                { -0.03f, 0.85f,  0.39f },
                {  0.93f, 0.03f,  0.00f },
                { -0.03f, 0.39f, -0.85f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },

    {
        .time = 160 * 1000,
        .state = {
            .position = { -2.48f, -0.08f, -1.67f },
            .orientation = {
                {  0.00f,  0.00f,  0.00f },
                {  0.00f,  0.00f,  0.00f },
                {  0.00f,  0.00f,  0.00f }
            },
            .box_scale = 0.0f,
            .box_radius = 0.0f,
            .sphere_radius = 0.0f
        }
    }
};

static const struct keypoint_text keypoint_texts[] =
{
    {
        .time = 0 * 1000,
        .text = _("intrusion 0.13.1 loaded\n> "),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 2 * 1000,
        .text = _("connect\n"),
        .type = CONSOLE_TEXT_TYPE_INPUT
    },
    {
        .time = 3 * 1000,
        .text = _("connecting...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 13 * 1000,
        .text = _("done\n> "),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 18 * 1000,
        .text = _("sysenter\n"),
        .type = CONSOLE_TEXT_TYPE_INPUT
    },
    {
        .time = 20 * 1000,
        .text = _("searching entrypoint...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 58 * 1000,
        .text = _("entering...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 81 * 1000,
        .text = _("done\n> "),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 88 * 1000,
        .text = _("locate sg-245\n"),
        .type = CONSOLE_TEXT_TYPE_INPUT
    },
    {
        .time = 89 * 1000,
        .text = _("searching data...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 100 * 1000,
        .text = _("trace detected!\nsearching data...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 105 * 1000,
        .text = _("trace distance 25%\nsearching data...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 120 * 1000,
        .text = _("trace distance 50%\nsearching data...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 124 * 1000,
        .text = _("^C\n> "),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 126 * 1000,
        .text = _("evade\n"),
        .type = CONSOLE_TEXT_TYPE_INPUT
    },
    {
        .time = 128 * 1000,
        .text = _("evading tracer...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 132 * 1000,
        .text = _("trace distance 25%\nevading tracer...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 138 * 1000,
        .text = _("trace distance 50%\nevading tracer...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 140 * 1000,
        .text = _("trace distance 75%\nevading tracer...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 145 * 1000,
        .text = _("trace distance 90%\nevading tracer...\n"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 148 * 1000,
        .text = _("^C\n> "),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 150 * 1000,
        .text = _("disconnect\n"),
        .type = CONSOLE_TEXT_TYPE_INPUT
    },
    {
        .time = 156 * 1000,
        .text = _("done\n> "),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    },
    {
        .time = 158 * 1000,
        .text = _("fuck it\n"),
        .type = CONSOLE_TEXT_TYPE_INPUT
    },
    {
        .time = 159 * 1000,
        .text = _("command not found"),
        .type = CONSOLE_TEXT_TYPE_OUTPUT
    }
};

#endif

