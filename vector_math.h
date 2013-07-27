#ifndef VECTOR_MATH_H_
#define VECTOR_MATH_H_

#include <math.h>

typedef float vector3[3];

#define VECTOR3_NULL (vector3){ 0.0, 0.0, 0.0 }
#define VECTOR3_UNIT_X (vector3){ 1.0, 0.0, 0.0 }
#define VECTOR3_UNIT_Y (vector3){ 0.0, 1.0, 0.0 }
#define VECTOR3_UNIT_Z (vector3){ 0.0, 0.0, 1.0 }
#define VECTOR3_NEGATIVE_UNIT_X (vector3){ -1.0,  0.0,  0.0 }
#define VECTOR3_NEGATIVE_UNIT_Y (vector3){  0.0, -1.0,  0.0 }
#define VECTOR3_NEGATIVE_UNIT_Z (vector3){  0.0,  0.0, -1.0 }

typedef enum vector3_axis
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z
} vector3_axis;

void vector3_add(vector3 vector, const vector3 addend);
void vector3_multiply(vector3 vector, float factor);
void vector3_copy(vector3 vector, const vector3 source);

#endif /* VECTOR_MATH_H_ */

