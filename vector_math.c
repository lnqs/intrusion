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

void vector3_rotate(vector3 vector, const matrix3 rotation)
{
    vector3 product;
    for (size_t row = 0; row < 3; row++)
    {
        product[row] = rotation[row][0] * vector[0]
                     + rotation[row][1] * vector[1]
                     + rotation[row][2] * vector[2];
    }

    vector3_copy(vector, product);
}

void vector3_copy(vector3 vector, const vector3 source)
{
    memcpy(vector, source, sizeof(vector3));
}

void matrix3_multiply(matrix3 matrix, const matrix3 other)
{
    matrix3 product;

    for (size_t row = 0; row < 3; row++)
    {
        for (size_t col = 0; col < 3; col++)
        {
            product[row][col] = matrix[row][0] * other[0][col]
                              + matrix[row][1] * other[1][col]
                              + matrix[row][2] * other[2][col];
        }
    }

    matrix3_copy(matrix, product);
}

void matrix3_rotate(matrix3 matrix, vector3 axis, float degree)
{
    float x = axis[AXIS_X];
    float y = axis[AXIS_Y];
    float z = axis[AXIS_Z];

    float sina = sin(degree);
    float cosa = cos(degree);

    matrix3 rotation = {
        { x * x * (1.0 - cosa) +     cosa, x * y * (1.0 - cosa) - z * sina, x * z * (1.0 - cosa) + y * sina },
        { y * x * (1.0 - cosa) + z * sina, y * y * (1.0 - cosa) +     cosa, y * z * (1.0 - cosa) - x * sina },
        { z * x * (1.0 - cosa) - y * sina, z * y * (1.0 - cosa) + x * sina, z * z * (1.0 - cosa) +     cosa }
    };

    matrix3_multiply(matrix, rotation);
}

void matrix3_copy(matrix3 matrix, const matrix3 source)
{
    memcpy(matrix, source, sizeof(matrix3));
}

