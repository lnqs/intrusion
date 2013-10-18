#ifndef DEFINES_H
#define DEFINED_H

#define RESOLUTION_X 800
#define RESOLUTION_Y 600
#define FULLSCREEN false
#define WINDOW_RATIO ((float)RESOLUTION_X / RESOLUTION_Y)

// Escape from beeing processed by preprocessor.
// Needed for the version-directives in the shaders
#define ESCAPED(x) x

#endif

