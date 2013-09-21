#ifndef VECTOR_MATH_H_
#define VECTOR_MATH_H_

#include <math.h>

typedef float vector3[3];
typedef float matrix3[3][3]; // [row][col]

#define VECTOR3_NULL { 0.0, 0.0, 0.0 }
#define VECTOR3_UNIT_X { 1.0, 0.0, 0.0 }
#define VECTOR3_UNIT_Y { 0.0, 1.0, 0.0 }
#define VECTOR3_UNIT_Z { 0.0, 0.0, 1.0 }
#define VECTOR3_NEGATIVE_UNIT_X { -1.0,  0.0,  0.0 }
#define VECTOR3_NEGATIVE_UNIT_Y {  0.0, -1.0,  0.0 }
#define VECTOR3_NEGATIVE_UNIT_Z {  0.0,  0.0, -1.0 }

#define MATRIX3_IDENTITY \
    { \
        { 1.0, 0.0, 0.0 }, \
        { 0.0, 1.0, 0.0 }, \
        { 0.0, 0.0, 1.0 }, \
    }

typedef enum vector3_axis
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z
} vector3_axis;

void vector3_add(vector3 vector, const vector3 addend);
void vector3_multiply(vector3 vector, float factor);
void vector3_rotate(vector3 vector, const matrix3 rotation);
void vector3_copy(vector3 vector, const vector3 source);

void matrix3_multiply(matrix3 matrix, const matrix3 other);
void matrix3_rotate(matrix3 matrix, vector3 axis, float degree);
void matrix3_copy(matrix3 matrix, const matrix3 source);

#endif /* VECTOR_MATH_H_ */

