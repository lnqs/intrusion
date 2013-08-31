#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include "vector_math.h"

GLuint compile_program(const char* vertex_source, const char* fragment_source);
void uniform_float(GLuint program, const char* identifier, float value);
void uniform_vector3(GLuint program, const char* identifier, const vector3 value);
void uniform_matrix3(GLuint program, const char* identifier, const matrix3 value);

#endif /* SHADER_H_ */

