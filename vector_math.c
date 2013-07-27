#include "vector_math.h"
#include <string.h>

void vector3_add(vector3 vector, const vector3 addend)
{
    vector[0] += addend[0];
    vector[1] += addend[1];
    vector[2] += addend[2];
}

void vector3_multiply(vector3 vector, float factor)
{
    vector[0] *= factor;
    vector[1] *= factor;
    vector[2] *= factor;
}

void vector3_copy(vector3 vector, const vector3 source)
{
    memcpy(vector, source, sizeof(vector3));
}

