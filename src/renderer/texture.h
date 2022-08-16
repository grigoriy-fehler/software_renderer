#ifndef TEXTURE_H
#define TEXTURE_H

#include "../math/mathlib.h"

#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define texture(width, height, data) (texture_t) { \
	(i32) (width), \
	(i32) (height), \
	(color_rgba_t *) (data) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct texture_t {
	i32 width;
	i32 height;
	color_rgba_t* data;
} texture_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void texture_flip_horizontal(texture_t* tex) {
	i32 half_width = tex->width >> 1;
	for (i32 y = 0; y < tex->height; y++) {
		for (i32 x = 0; x < half_width; x++) {
			color_swap(
				&tex->data[y * tex->width + x],
				&tex->data[y * tex->width + tex->width - 1 - x]
			);
		}
	}
} // texture_flip_horizontal

static inline void texture_flip_vertical(texture_t* tex) {
	i32 half_height = tex->height >> 1;
	i32 size = tex->width * tex->height;
	i32 bottom_line = size - tex->width;
	for (i32 y = 0; y < half_height; y++) {
		for (i32 x = 0; x < tex->width; x++) {
			color_swap(
				&tex->data[y * tex->width + x],
				&tex->data[bottom_line - y * tex->width + x]
			);
		}
	}
} // texture_flip_vertical

#endif // TEXTURE_H
