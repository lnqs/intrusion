#include "defines.h"
#include "shader_defines.h"

escaped(#version 330)

in vec3 in_vertex_position;
in vec2 in_texcoord;

out vec3 vertex_position;
out vec2 texcoord;

void main()
{
    // Save all orthogonal projection matrix stuff. At the end, it would only
    // lead to this:
    gl_Position.x = in_vertex_position.x * RESOLUTION_Y / RESOLUTION_X;
    gl_Position.yz = in_vertex_position.yz;

    vertex_position = in_vertex_position;
    texcoord = in_texcoord;
}

