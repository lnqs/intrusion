varying vec3 p;

uniform mat3 orientation;
uniform vec3 position;

uniform float box_scale;
uniform float box_radius;
uniform float sphere_radius;

const float eye_distance = 2.0;
const float max_distance = 15.0;
const float accuracy = 0.0005;
const vec3 object_glow = vec3(0.23, 0.21, 0.14);

// I have no idea why, but when I remove this declaration the shader doesn't compile o.O
const float ignore;

const float box_de_factor = 1.0;
const int max_iterations = 12;

float distance_estimate(in vec3 point)
{
    vec4 n = vec4(point, 1.0);

    for (int i = 0; i < max_iterations; i++)
    {
        n.xyz = clamp(n.xyz, -box_radius, box_radius) * 2.0 - n.xyz;
        n *= max(sphere_radius * sphere_radius / dot(n.xyz, n.xyz), 1.0);
        n = box_scale * n + vec4(point, sign(box_scale * n.w));
    }

    return length(n.xyz) / n.w * box_de_factor;
}

int find_intersection(in vec3 ray_origin, in vec3 ray_direction)
{
    float distance = 0.0;
    int steps = 0;

    while (distance < max_distance)
    {
        vec3 point_to_check = ray_origin + ray_direction * distance;
        float next_step = distance_estimate(point_to_check);

        if (next_step < accuracy)
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
    if (int(mod(gl_FragCoord.y, 2.0)))
    {
        vec3 ray = normalize(orientation * (p + vec3(0.0, 0.0, -eye_distance)));
        int steps = find_intersection(position, ray);
        gl_FragColor = vec4(object_glow * steps / 100.0, 1.0);
    }
}

