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
            .position = { 0.00, 0.00, 4.30 },
            .orientation = {
                {  0.00,  0.00,  0.00 },
                {  0.00,  0.00,  0.00 },
                {  0.00,  0.00,  0.00 }
            },
            .box_scale = 0.0,
            .box_radius = 0.0,
            .sphere_radius = 0.0,
        }
    },
    {
        .time = 3 * 1000,
        .state = {
            .position = { 0.00, 0.00, 4.30 },
            .orientation = {
                {  0.00,  0.00,  0.00 },
                {  0.00,  0.00,  0.00 },
                {  0.00,  0.00,  0.00 }
            },
            .box_scale = 0.0,
            .box_radius = 0.0,
            .sphere_radius = 0.0,
        }
    },
    {
        .time = 13 * 1000,
        .state = {
            .position = { 0.23, -5.74, 7.96 },
            .orientation = {
                {  0.96, -0.04,  0.26 },
                {  0.24,  0.54, -0.80 },
                { -0.11,  0.83,  0.53 }
            },
            .box_scale = -5.0,
            .box_radius = 4.0,
            .sphere_radius = 3.0
        }
    },
    {
        .time = 20 * 1000,
        .state = {
            .position = { 2.51, -4.36, 6.77 },
            .orientation = {
                {  0.60, -0.00, -0.17 },
                {  0.26,  0.03, -0.54 },
                { -0.39,  0.41,  0.34 }
            },
            .box_scale = -4.1,
            .box_radius = 3.6,
            .sphere_radius = 2.2
        }
    },
    {
        .time = 30 * 1000,
        .state = {
            .position = { 5.09, -1.66, 5.23 },
            .orientation = {
                { -0.03,  0.08, -0.99 },
                {  0.35, -0.92, -0.09 },
                { -0.93, -0.35,  0.00 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = 0.75
        }
    },
    {
        .time = 40 * 1000,
        .state = {
            .position = { 5.06, 3.74, 5.29 },
            .orientation = {
                { -0.30,  0.93, -0.08 },
                {  0.04, -0.06, -0.98 },
                { -0.94, -0.31, -0.01 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = 0.75
        }
    },
    {
        .time = 50 * 1000,
        .state = {
            .position = { 5.08, 8.65, 5.31 },
            .orientation = {
                { -0.02,  0.85, 0.47 },
                { -0.69, -0.34, 0.60 },
                {  0.71, -0.32, 0.60 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = 0.75
        }
    },
    {
        .time = 60 * 1000,
        .state = {
            .position = { 0.33, 7.38, -1.03 },
            .orientation = {
                { 0.54,  0.76,  0.23 },
                { 0.17, -0.38,  0.86 },
                { 0.77, -0.44, -0.37 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 70 * 1000,
        .state = {
            .position = { -0.24, 5.19, -0.57 },
            .orientation = {
                { -0.04 , 0.90, -0.31 },
                {  0.93, -0.02, -0.19 },
                { -0.21, -0.31, -0.88 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 80 * 1000,
        .state = {
            .position = { -0.14, 3.67, 0.15 },
            .orientation = {
                { -0.04,  0.94, -0.05 },
                {  0.11, -0.04, -0.93 },
                { -0.94, -0.04, -0.09 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 90 * 1000,
        .state = {
            .position = { -0.25, 3.33, -0.70 },
            .orientation = {
                {  0.01,  0.93,  0.17 },
                { -0.94,  0.02, -0.05 },
                { -0.03, -0.17,  0.93 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 100 * 1000,
        .state = {
            .position = { -0.40, 3.62, -2.33 },
            .orientation = {
                { -0.05,  0.91,  0.24 },
                { -0.73,  0.11, -0.59 },
                { -0.58, -0.21,  0.71 }
            },
            .box_scale = -2.5,
            .box_radius = 3.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 110 * 1000,
        .state = {
            .position = { -0.69, 3.02, -3.72 },
            .orientation = {
                { -0.14,  0.65, -0.67 },
                {  0.18, -0.64, -0.67 },
                { -0.92, -0.21, -0.01 }
            },
            .box_scale = -2.5,
            .box_radius = 2.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 120 * 1000,
        .state = {
            .position = { -1.05, 2.77, -4.57 },
            .orientation = {
                {  0.50,  0.79, -0.05 },
                {  0.78, -0.49, -0.11 },
                { -0.15,  0.02, -0.92 }
            },
            .box_scale = -2.5,
            .box_radius = 2.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 130 * 1000,
        .state = {
            .position = { -1.33, -0.22, -4.94 },
            .orientation = {
                { -0.02,  0.66, -0.66 },
                {  0.93, -0.01, -0.06 },
                { -0.07, -0.66, -0.65 }
            },
            .box_scale = -2.5,
            .box_radius = 2.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 140 * 1000,
        .state = {
            .position = { -0.88, -0.14, -3.26 },
            .orientation = {
                { -0.01,  0.89,  0.28 },
                {  0.93,  0.02, -0.00 },
                { -0.03,  0.28, -0.89 }
            },
            .box_scale = -2.5,
            .box_radius = 2.0,
            .sphere_radius = -2.0
        }
    },
    {
        .time = 150 * 1000,
        .state = {
            .position = { -2.48, -0.08, -1.67 },
            .orientation = {
                { -0.03, 0.85,  0.39 },
                {  0.93, 0.03,  0.00 },
                { -0.03, 0.39, -0.85 }
            },
            .box_scale = -2.5,
            .box_radius = 2.0,
            .sphere_radius = -2.0
        }
    },

    {
        .time = 160 * 1000,
        .state = {
            .position = { -2.48, -0.08, -1.67 },
            .orientation = {
                {  0.00,  0.00,  0.00 },
                {  0.00,  0.00,  0.00 },
                {  0.00,  0.00,  0.00 }
            },
            .box_scale = 0.0,
            .box_radius = 0.0,
            .sphere_radius = 0.0
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

