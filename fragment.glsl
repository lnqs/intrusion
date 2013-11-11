#include "defines.h"
#include "shader_defines.h"

escaped(#version 330)

in vec3 position;
in vec2 texcoord;

out vec3 out_color;

// CPU passes camera-position and -orientation and fractal parameters as an
// array of vectors. We've got some defines below, to make the access more
// readable.
uniform vec3 uf_cpu_data[];

#define cam_position uf_cpu_data[0]
#define cam_orientation mat3(uf_cpu_data[1], uf_cpu_data[2], uf_cpu_data[3])
#define box_scale uf_cpu_data[4].x
#define box_radius uf_cpu_data[4].y
#define sphere_radius uf_cpu_data[4].z

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
            n.xyz = clamp(n.xyz, -box_radius, box_radius) * 2.0 - n.xyz;
            n *= max(sphere_radius * sphere_radius / dot(n.xyz, n.xyz), 1.0);
            n = box_scale * n + vec4(point, sign(box_scale * n.w));
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
        //     vec3 ray = normalize(cam_orientation * (position + vec3(0.0, 0.0, -EYE_DISTANCE)));
        //     int steps = find_intersection(cam_position, ray);
        //     c = OBJECT_GLOW * steps / 85.0;
        // but smaller.
        out_color = OBJECT_GLOW * find_intersection(cam_position,
            normalize(cam_orientation *
                (position + vec3(0.0, 0.0, -EYE_DISTANCE)))) / 85.0;
    }
}

