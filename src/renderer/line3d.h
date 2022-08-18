#ifndef LINE3D_H
#define LINE3D_H

#include "../math/mathlib.h"

#include "vertex3d.h"
#include "framebuffer.h"
#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define line3d(start, end) (line3d_t) { \
	(vertex3d_t) (start), \
	(vertex3d_t) (end) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct line3d_t {
	vertex3d_t start, end;
} line3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void line3d_stroke(framebuffer_t* fb, line3d_t* line) {
	i32 x_start = line->start.position.x;
	i32 y_start = line->start.position.y;
	i32 x_end = line->end.position.x;
	i32 y_end = line->end.position.y;
	color_rgba_t color = line->start.color;

	i32 dx = x_end - x_start;
	i32 dy = y_end - y_start;
	i32 x_incr = (dx >= 0) ? 1 : -1;
	i32 y_incr = (dy >= 0) ? 1 : -1;

	dx = absolute(dx);
	dy = absolute(dy);

	i32 x = x_start;
	i32 y = y_start;
	i32 error = 0;
	if (dx > dy) {
		for (i32 i = 0; i < dx; i++) {
			set_pixel(fb, x, y, &color);
			x += x_incr;
			error += dy;
			if (error > dx) {
				y += y_incr;
				error -= dx;
			}
		}
	} else {
		for (i32 i = 0; i < dy; i++) {
			set_pixel(fb, x, y, &color);
			y += y_incr;
			error += dx;
			if (error > dy) {
				x += x_incr;
				error -= dy;
			}
		}
	}
} // line3d_stroke

#endif // LINE3D_H
