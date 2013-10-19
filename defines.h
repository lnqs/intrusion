#ifndef DEFINES_H
#define DEFINES_H

// Some global compile-time options.
// As preprocessor-defines, since some of them are used in shader-code

#define RESOLUTION_X 800
#define RESOLUTION_Y 600
#define FULLSCREEN false
#define WINDOW_RATIO ((float)RESOLUTION_X / RESOLUTION_Y)

#define MAX_SKEW 0.75
#define SKEW_DECREASING_MULTIPLIER 0.01

#define GLYPH_SPACING 4
#define GLYPH_SCALE 2.0
#define TEXT_START_X 10
#define TEXT_START_Y 10

#define EYE_DISTANCE 2.0
#define MAX_DISTANCE 15.0
#define ACCURACY 0.0005
#define OBJECT_GLOW vec3(0.23, 0.21, 0.14)

#define MAX_ITERATIONS 12

// Noop :o)
// While we don't want this to do anything in the code, build_glyphs.py uses it
// when parsing the source, to see that a string is going to be displayed and
// the glyphs it uses have to be included
#define _(x) x

#endif

