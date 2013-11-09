#include "defines.h"
#include "shader_defines.h"

escaped(#version 330)

in vec3 position;
in vec2 texcoord;

out vec3 out_color;

uniform mat3 uf_cam_orientation;
uniform vec3 uf_cam_position;
uniform vec3 uf_fractal_params; // box_scale, box_radius, sphere_radius

// This is never set in the CPU-code, to save some code. Some searching on the
// internet gave me the impression, that it is reliable initialized to the first
// unit implicitly, but, well, I didn't find a clear statement. So let's just
// cross fingers.
uniform sampler2D uf_overlay_texture;

int find_intersection(in vec3 ray_origin, in vec3 ray_direction)
{
    float dist = 0.0;
    int steps = 0;

    while (dist < MAX_DISTANCE)
    {
        // Here starts, what was the function distance_estimate() once.
        // Due to a bug in shader_minifier, which was using the same name for
        // two functions, I had to inline it.
        // The original function took 'point' as parameter...
        vec3 point = ray_origin + ray_direction * dist;
        vec4 n = vec4(point, 1.0);

        for (int i = 0; i < MAX_ITERATIONS; i++)
        {
            n.xyz = clamp(n.xyz, -uf_fractal_params.y, uf_fractal_params.y) * 2.0 - n.xyz;
            n *= max(uf_fractal_params.z * uf_fractal_params.z / dot(n.xyz, n.xyz), 1.0);
            n = uf_fractal_params.x * n + vec4(point, sign(uf_fractal_params.x * n.w));
        }

        // ... and returned this rhs expression, that was assign to 'next_step'
        // by the caller.
        float next_step = length(n.xyz) / n.w;

        if (next_step < ACCURACY)
        {
            return steps;
        }

        dist += next_step;
        steps++;
    }

    return steps;
}

void main()
{
    out_color = vec3(texture2D(uf_overlay_texture, texcoord).r);

    if (out_color.r == 0.0 && int(mod(gl_FragCoord.y, 3.0)) == 0)
    {
        // Since the following line of code isn't really readable -- it's the same as this
        //     vec3 ray = normalize(uf_cam_orientation * (position + vec3(0.0, 0.0, -EYE_DISTANCE)));
        //     int steps = find_intersection(uf_cam_position, ray);
        //     c = OBJECT_GLOW * steps / 85.0;
        // but smaller.
        out_color = OBJECT_GLOW * find_intersection(uf_cam_position,
            normalize(uf_cam_orientation *
                (position + vec3(0.0, 0.0, -EYE_DISTANCE)))) / 85.0;
    }
}

