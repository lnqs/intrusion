#include "defines.h"

ESCAPED(#version 330)

in vec3 p; // vertex position
in vec2 r; // tex coord

out vec3 c; // fragment color

uniform vec3 x; // position passed by CPU-code
uniform mat3 o; // orientation passed by CPU-code
uniform vec3 f; // fractal-parameters passed by CPU-code. It's (box_scale, box_radius, sphere_radius).
uniform vec3 e; // effect-parameters. Currently only .x is used, for the 'skew-multiplier'
uniform sampler2D t; // text-rendering texture

// mutilated version of Marsaglia's MWC random number generator
float rand(in vec2 seed)
{
    return float((36969u * uint(seed.x) << 16u) + (18000u * uint(seed.y))) * 2.328306435454494e-10;
}

float distance_estimate(in vec3 point)
{
    vec4 n = vec4(point, 1.0);

    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        n.xyz = clamp(n.xyz, -f.y, f.y) * 2.0 - n.xyz;
        n *= max(f.z * f.z / dot(n.xyz, n.xyz), 1.0);
        n = f.x * n + vec4(point, sign(f.x * n.w));
    }

    return length(n.xyz) / n.w;
}

int find_intersection(in vec3 ray_origin, in vec3 ray_direction)
{
    float dist = 0.0;
    int steps = 0;

    while (dist < MAX_DISTANCE)
    {
        float next_step = distance_estimate(ray_origin + ray_direction * dist);

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
    c = vec3(texture2D(t, r).r);

    if (c.r == 0.0 && int(mod(gl_FragCoord.y, 2.0)) == 0)
    {
        // Since the following line of code isn't really readable -- it's the same as this:
        //     float skew = rand(gl_FragCoord.xy) * e.x + 1.0 - e.x;
        //     vec3 ray = normalize(o * (p * skew + vec3(0.0, 0.0, -EYE_DISTANCE)));
        //     int steps = find_intersection(x, ray);
        //     c = OBJECT_GLOW * steps / 85.0;
        c = OBJECT_GLOW * find_intersection(x,
            normalize(o *
                (p * (rand(gl_FragCoord.xy) * e.x + 1.0 - e.x)
                    + vec3(0.0, 0.0, -EYE_DISTANCE)))) / 85.0;
    }
}

