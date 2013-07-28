varying vec3 p;

uniform mat3 orientation;
uniform vec3 position;

const float eye_distance = 2.0;
const float max_distance = 5.0;
const float accuracy = 0.0005;

const float normal_eps = 0.0005;

// We could define this in CPU-code with the GL-lighting-functions,
// but I don't see a benefit while it would be more code.
vec3 light_position = position; // Not const since position isn't
const vec3 light_diffuse = vec3(0.5, 0.5, 0.5);
const vec3 light_specular = vec3(0.5, 0.5, 0.5);

const vec3 object_color = vec3(1.0, 1.0, 0.66);
const float object_shininess = 10.0;

const float box_scale = -2.5;
const float box_radius = 1.0;
const float sphere_radius = 1.5;
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

vec3 find_normal(in vec3 point)
{
    vec2 epsilon = vec2(normal_eps, 0.0);

    vec3 normal;
    normal.x = distance_estimate(point - epsilon.xyy) - distance_estimate(point + epsilon.xyy);
    normal.y = distance_estimate(point - epsilon.yxy) - distance_estimate(point + epsilon.yxy);
    normal.z = distance_estimate(point - epsilon.yyx) - distance_estimate(point + epsilon.yyx);
    normal = normalize(normal);

    return normal;
}

bool intersects(in vec3 ray_origin, in vec3 ray_direction, out vec3 hitpoint, out vec3 normal, out int steps)
{
    float distance = 0.0;
    steps = 0;

    while (distance < max_distance)
    {
        vec3 point_to_check = ray_origin + ray_direction * distance;
        float next_step = distance_estimate(point_to_check);

        if (next_step < accuracy)
        {
            hitpoint = point_to_check;
            normal = find_normal(hitpoint);
            return true;
        }

        distance += next_step;
        steps++;
    }

    return false;
}

void light(in vec3 point, in vec3 normal, inout vec3 color)
{
    vec3 light = normalize(light_position - point);
    vec3 diffuse = max(dot(normal, -light), 0.0) * light_diffuse;
    vec3 specular = light_specular * pow(max(dot(normalize(reflect(light, normal)), -light), 0.0), object_shininess);
    color *= diffuse + specular;
}

void main()
{
    vec3 ray = normalize(orientation * (p + vec3(0.0, 0.0, -eye_distance)));

    vec3 hitpoint;
    vec3 normal;
    int steps;
    if (intersects(position, ray, hitpoint, normal, steps))
    {
        vec3 color = object_color;
        light(hitpoint, normal, color);
        gl_FragColor = vec4(color.x, color.y, color.z, 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

