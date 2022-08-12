#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../math/mathlib.h"

#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct framebuffer_t {
	image_format_t image_format;
	i32 width;
	i32 height;
	f32* depth;
	u32* color;
} framebuffer_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void clear_color(framebuffer_t* fb, color_rgba_t* color) {
	u32 c = color_to_u32(color, fb->image_format);
	for (i32 i = 0; i < fb->width * fb->height; i++)
		fb->color[i] = c;
} // clear_color

static inline void clear_depth(framebuffer_t* fb, f32 depth) {
	for (i32 i = 0; i < fb->width * fb->height; i++)
		fb->depth[i] = depth;
} // clear_depth

static inline void set_pixel(framebuffer_t* fb, i32 x, i32 y,
	color_rgba_t* color)
{
	u32 c = color_to_u32(color, fb->image_format);
	fb->color[y * fb->width + x] = c;
} // set_pixel

static inline void set_depth(framebuffer_t* fb, i32 x, i32 y, f32 depth) {
	fb->depth[y * fb->width + x] = depth;
} // set_depth

static inline f32 get_depth(framebuffer_t* fb, i32 x, i32 y) {
	if (x < 0 || x >= fb->width) return -1.0f;
	if (y < 0 || y >= fb->height) return -1.0f;
	return fb->depth[y * fb->width + x];
} // get_depth

#endif // FRAMEBUFFER_H
