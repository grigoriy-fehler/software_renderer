#ifndef COLOR_RGBA_H
#define COLOR_RGBA_H

#include "../math/mathlib.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define color_rgba(r, g, b, a) (color_rgba_t) {{ \
	(f32) (r), \
	(f32) (g), \
	(f32) (b), \
	(f32) (a) \
}}

// E N U M S ///////////////////////////////////////////////////////////////////

typedef enum image_format_t {
	IMAGE_FORMAT_RGBA = 0,
	IMAGE_FORMAT_ARGB,
	IMAGE_FORMAT_ABGR
} image_format_t;

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union color_rgba_t {
	struct { f32 r, g, b, a; };
	vector4d_t rgba;
	vector3d_t rgb;
	f32 e[4];
} color_rgba_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void color_swap(color_rgba_t* color1, color_rgba_t* color2) {
	swapf(&color1->r, &color2->r);
	swapf(&color1->g, &color2->g);
	swapf(&color1->b, &color2->b);
	swapf(&color1->a, &color2->a);
} // color_swap

static inline u32 color_to_rgba(color_rgba_t* color) {
	u32 r = color->r * 255.0f;
	u32 g = color->g * 255.0f;
	u32 b = color->b * 255.0f;
	u32 a = color->a * 255.0f;
	return r << 24 | g << 16 | b << 8 | a;
} // color_to_rgba

static inline u32 color_to_argb(color_rgba_t* color) {
	u32 a = color->a * 255.0f;
	u32 r = color->r * 255.0f;
	u32 g = color->g * 255.0f;
	u32 b = color->b * 255.0f;
	return a << 24 | r << 16 | g << 8 | b;
} // color_to_argb

static inline u32 color_to_abgr(color_rgba_t* color) {
	u32 a = color->a * 255.0f;
	u32 b = color->b * 255.0f;
	u32 g = color->g * 255.0f;
	u32 r = color->r * 255.0f;
	return a << 24 | b << 16 | g << 8 | r;
} // color_to_abgr

static inline void color_from_rgba(color_rgba_t* out, u32 color) {
	u8 r = color >> 24;
	u8 g = color >> 16;
	u8 b = color >> 8;
	u8 a = color;
	out->r = (f32) r / 255.0f;
	out->g = (f32) g / 255.0f;
	out->b = (f32) b / 255.0f;
	out->a = (f32) a / 255.0f;
} // color_from_rgba

static inline void color_from_argb(color_rgba_t* out, u32 color) {
	u8 a = color >> 24;
	u8 r = color >> 16;
	u8 g = color >> 8;
	u8 b = color;
	out->a = (f32) a / 255.0f;
	out->r = (f32) r / 255.0f;
	out->g = (f32) g / 255.0f;
	out->b = (f32) b / 255.0f;
} // color_from_argb

static inline u32 color_to_u32(color_rgba_t* color, image_format_t image_format)
{
	u32 c;
	switch (image_format) {
		case IMAGE_FORMAT_RGBA:
			c = color_to_rgba(color);
			break;
		case IMAGE_FORMAT_ARGB:
			c = color_to_argb(color);
			break;
		case IMAGE_FORMAT_ABGR:
			c = color_to_abgr(color);
			break;
		default:
			c = color_to_rgba(color);
			break;
	}
	return c;
} // color_to_u32

#endif // COLOR_RGBA_H
