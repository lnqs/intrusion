#include "defines.h"

ESCAPED(#version 330)

#define EYE_DISTANCE 2.0
#define MAX_DISTANCE 15.0
#define ACCURACY 0.0005
#define OBJECT_GLOW vec3(0.23, 0.21, 0.14)

#define MAX_ITERATIONS 12

in vec3 p; // vertex position
out vec3 c; // fragment color

uniform vec3 x; // position passed by CPU-code
uniform mat3 o; // orientation passed by CPU-code
uniform vec3 f; // fractal-parameters passed by CPU-code. It's (box_scale, box_radius, sphere_radius).

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
    float distance = 0.0;
    int steps = 0;

    while (distance < MAX_DISTANCE)
    {
        float next_step = distance_estimate(ray_origin + ray_direction * distance);

        if (next_step < ACCURACY)
        {
            return steps;
        }

        distance += next_step;
        steps++;
    }

    return steps;
}

void main()
{
    if (int(mod(gl_FragCoord.y, 2.0)) == 0)
    {
        // Since the following line of code isn't really readable -- it's the same as this:
        //     vec3 ray = normalize(o * (p + vec3(0.0, 0.0, -EYE_DISTANCE)));
        //     int steps = find_intersection(x, ray);
        //     c = OBJECT_GLOW * steps / 85.0;

        c = OBJECT_GLOW * find_intersection(x, normalize(o * (p + vec3(0.0, 0.0, -EYE_DISTANCE)))) / 85.0;
    }
}

