#ifndef DEFINES_H
#define DEFINES_H

// Escape from beeing processed by preprocessor.
// Needed for the version-directives in the shaders
#define ESCAPED(x) x

#define RESOLUTION_X 800
#define RESOLUTION_Y 600
#define FULLSCREEN false
#define WINDOW_RATIO ((float)RESOLUTION_X / RESOLUTION_Y)

#define EYE_DISTANCE 2.0
#define MAX_DISTANCE 15.0
#define ACCURACY 0.0005
#define OBJECT_GLOW vec3(0.23, 0.21, 0.14)

#define MAX_ITERATIONS 12

#endif

