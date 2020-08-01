#include "color.h"

uint32_t color_to_uint(Color c) {
	uint32_t r = c.r;
	r = r << 24;

	uint32_t g = c.g;
	g = g << 16;

	uint32_t b = c.b;
	b = b << 8;

	uint32_t a = c.a;

	return r | g | b | a;
}
