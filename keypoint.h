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
            .position = { 0.0f, 0.0f, 4.3f },
            .orientation = {
                {  0.0f,  0.0f,  0.0f },
                {  0.0f,  0.0f,  0.0f },
                {  0.0f,  0.0f,  0.0f }
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
                {  0.0f,  0.0f,  0.0f },
                {  0.0f,  0.0f,  0.0f },
                {  0.0f,  0.0f,  0.0f }
            },
            .box_scale = 0.0f,
            .box_radius = 0.0f,
            .sphere_radius = 0.0f,
        }
    },
    {
        .time = 13 * 1000,
        .state = {
            .position = { 0.2f, -5.7f, 7.9f },
            .orientation = {
                {  0.9f,  0.2f, -0.1f },
                { -0.0f,  0.5f,  0.8f },
                {  0.2f, -0.8f,  0.5f }
            },
            .box_scale = -5.0f,
            .box_radius = 4.0f,
            .sphere_radius = 3.0f
        }
    },
    {
        .time = 20 * 1000,
        .state = {
            .position = { 2.5f, -4.3f, 6.7f },
            .orientation = {
                {  0.6f,  0.2f, -0.3f },
                { -0.0f,  0.0f,  0.4f },
                { -0.1f, -0.5f,  0.3f }
            },
            .box_scale = -4.1f,
            .box_radius = 3.6f,
            .sphere_radius = 2.2f
        }
    },
    {
        .time = 30 * 1000,
        .state = {
            .position = { 5.0f, -1.6f, 5.2f },
            .orientation = {
                { -0.0f,  0.3f, -0.9f },
                {  0.0f, -0.9f, -0.3f },
                { -0.9f, -0.0f,  0.0f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = 0.7f
        }
    },
    {
        .time = 40 * 1000,
        .state = {
            .position = { 5.0f, 3.7f, 5.2f },
            .orientation = {
                { -0.3f,  0.0f, -0.9f },
                {  0.9f, -0.0f, -0.3f },
                { -0.0f, -0.9f, -0.0f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = 0.7f
        }
    },
    {
        .time = 50 * 1000,
        .state = {
            .position = { 5.0f, 8.6f, 5.3f },
            .orientation = {
                { -0.0f,  0.8f, 0.4f },
                { -0.6f, -0.3f, 0.6f },
                {  0.7f, -0.3f, 0.6f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = 0.7f
        }
    },
    {
        .time = 60 * 1000,
        .state = {
            .position = { 0.3f, 7.3f, -1.0f },
            .orientation = {
                { 0.5f,  0.7f,  0.2f },
                { 0.1f, -0.3f,  0.8f },
                { 0.7f, -0.4f, -0.3f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 70 * 1000,
        .state = {
            .position = { -0.2f, 5.1f, -0.5f },
            .orientation = {
                { -0.0f,  0.9f, -0.2f },
                {  0.9f, -0.0f, -0.3f },
                { -0.3f, -0.1f, -0.8f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 80 * 1000,
        .state = {
            .position = { -0.1f, 3.6f, 0.1f },
            .orientation = {
                { -0.0f,  0.1f, -0.9f },
                {  0.9f, -0.0f, -0.0f },
                { -0.0f, -0.9f, -0.0f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 90 * 1000,
        .state = {
            .position = { -0.2f, 3.3f, -0.7f },
            .orientation = {
                {  0.0f, -0.9f, -0.0f },
                {  0.9f,  0.0f, -0.1f },
                {  0.1f, -0.0f,  0.9f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 100 * 1000,
        .state = {
            .position = { -0.4f, 3.6f, -2.3f },
            .orientation = {
                { -0.0f, -0.7f, -0.5f },
                {  0.9f,  0.1f, -0.2f },
                {  0.2f, -0.5f,  0.7f }
            },
            .box_scale = -2.5f,
            .box_radius = 3.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 110 * 1000,
        .state = {
            .position = { -0.6f, 3.0f, -3.7f },
            .orientation = {
                { -0.1f,  0.1f, -0.9f },
                {  0.6f, -0.6f, -0.2f },
                { -0.6f, -0.6f, -0.0f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 120 * 1000,
        .state = {
            .position = { -1.0f, 2.7f, -4.5f },
            .orientation = {
                {  0.5f,  0.7f, -0.1f },
                {  0.7f, -0.4f,  0.0f },
                { -0.0f, -0.1f, -0.9f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 130 * 1000,
        .state = {
            .position = { -1.3f, -0.2f, -4.9f },
            .orientation = {
                { -0.0f,  0.9f, -0.0f },
                {  0.6f, -0.0f, -0.6f },
                { -0.6f, -0.0f, -0.6f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 140 * 1000,
        .state = {
            .position = { -0.8f, -0.1f, -3.2f },
            .orientation = {
                { -0.0f,  0.9f, -0.0f },
                {  0.8f,  0.0f,  0.2f },
                {  0.2f, -0.0f, -0.8f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },
    {
        .time = 150 * 1000,
        .state = {
            .position = { -2.4f, -0.0f, -1.6f },
            .orientation = {
                { -0.0f, 0.8f,  0.3f },
                {  0.9f, 0.0f,  0.0f },
                { -0.0f, 0.3f, -0.8f }
            },
            .box_scale = -2.5f,
            .box_radius = 2.0f,
            .sphere_radius = -2.0f
        }
    },

    {
        .time = 160 * 1000,
        .state = {
            .position = { -2.4f, -0.0f, -1.6f },
            .orientation = {
                {  0.0f,  0.0f,  0.0f },
                {  0.0f,  0.0f,  0.0f },
                {  0.0f,  0.0f,  0.0f }
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

