#include "defines.h"

ESCAPED(#version 330)

in vec3 v; // vertex position
out vec3 p; // pass vertex position to fragment shader

void main()
{
    // Save all orthogonal projection matrix stuff. At the end, it would only
    // lead to this:
    gl_Position.x = v.x * RESOLUTION_Y / RESOLUTION_X;
    gl_Position.yz = v.yz;
    p = v;
}

