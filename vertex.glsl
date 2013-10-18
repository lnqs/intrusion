#include "defines.h"

ESCAPED(#version 330)

in vec4 vertex;
out vec3 p;

void main()
{
    gl_Position.x = vertex.x * (float(RESOLUTION_Y) / RESOLUTION_X);
    gl_Position.yz = vertex.yz;
    p = vertex.xyz;
}
