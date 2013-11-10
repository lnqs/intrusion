#ifndef SHADER_DEFINES
#define SHADER_DEFINES

// This file contains some defines for better readability of the shader-code,
// while not wasting bytes for long global identifiers shader minifier cannot
// rename.

// Escape from beeing processed by preprocessor.
// Needed for the version-directives in the shaders
#define escaped(x) x

#define uf_cam_position a
#define uf_cam_orientation b
#define uf_fractal_params c
#define uf_effect_params d
#define uf_overlay_texture e

#define in_position f
#define in_texcoord g

#define position h
#define texcoord i

#define out_color j

// To stringify the identifiers defined above. Two levels are needed, since
// the preprocessor doesn't expand macros when using the stringify-operator
#define _str(s) #s
#define uniform(identifier) _str(identifier)

#endif

