varying vec3 p;

uniform vec3 x; // position passed by CPU-code
uniform mat3 o; // orientation passed by CPU-code
uniform vec3 f; // fractal-parameters passed by CPU-code. It's (box_scale, box_radius, sphere_radius).

const float eye_distance = 2.0;
const float max_distance = 15.0;
const float accuracy = 0.0005;
const vec3 object_glow = vec3(0.23, 0.21, 0.14);

const float box_de_factor = 1.0;
const int max_iterations = 12;

float distance_estimate(in vec3 point)
{
    vec4 n = vec4(point, 1.0);

    for (int i = 0; i < max_iterations; i++)
    {
        n.xyz = clamp(n.xyz, -f.y, f.y) * 2.0 - n.xyz;
        n *= max(f.z * f.z / dot(n.xyz, n.xyz), 1.0);
        n = f.x * n + vec4(point, sign(f.x * n.w));
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
        vec3 ray = normalize(o * (p + vec3(0.0, 0.0, -eye_distance)));
        int steps = find_intersection(x, ray);
        gl_FragColor = vec4(object_glow * steps / 85.0, 1.0);
    }
}

