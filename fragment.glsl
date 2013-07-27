varying vec3 p;

const vec3 position = vec3(0.0, 0.0, 2.5);
const mat3 orientation = mat3(1.0);

const float eye_distance = 2.0;
const float max_distance = 15.0;
const float step_length = 0.1;

const vec3 sphere_position = vec3(1.0, 0.0, 0.0);
const float sphere_radius = 0.25;

bool intersects(in vec3 ray_origin, in vec3 ray_direction)
{
    for (float dist = 0.0; dist < max_distance; dist += step_length)
    {
        vec3 point_to_check = ray_origin + (ray_direction * dist);

        if (length(point_to_check + sphere_position) <= sphere_radius)
        {
            return true;
        }
    }

    return false;
}

void main()
{
    vec3 ray = normalize(orientation * (p + vec3(0.0, 0.0, -eye_distance)));

    if (intersects(position, ray))
    {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

