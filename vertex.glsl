#include "defines.h"

ESCAPED(#version 330)

in vec4 v; // vertex
out vec3 p; // pass vertex to fragment shader

void main()
{
    gl_Position.x = v.x * (float(RESOLUTION_Y) / RESOLUTION_X);
    gl_Position.yz = v.yz;
    p = v.xyz;
}
