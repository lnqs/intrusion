#ifndef SHADER_DEFINES
#define SHADER_DEFINES

// This file contains some defines for better readability of the shader-code,
// while not wasting bytes for long global identifiers shader minifier cannot
// rename.

// Escape from beeing processed by preprocessor.
// Needed for the version-directives in the shaders
#define escaped(x) x

#define uf_cam_position x
#define uf_cam_orientation o
#define uf_fractal_params f
#define uf_effect_params e
#define uf_text_texture t

#define in_vertex_position v
#define in_texcoord j

#define vertex_position p
#define texcoord r

#define out_color c

// To stringify the identifiers defined above. Two levels are needed, since
// the preprocessor doesn't expand macros when using the stringify-operator
#define _str(s) #s
#define uniform(identifier) _str(identifier)

#endif

