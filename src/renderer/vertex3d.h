#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "../math/mathlib.h"

#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define vertex3d(position, texcoord, color) (vertex3d_t) { \
	(point4d_t) (position), \
	(point2d_t) (texcoord), \
	(color_rgba_t) (color) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct vertex3d_t {
	point4d_t position;
	point2d_t texcoord;
	color_rgba_t color;
} vertex3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void vertex3d_transform(vertex3d_t* out, vertex3d_t* in,
	matrix4x4_t* matrix)
{
	vector4d_multiply_matrix4x4(&out->position, &in->position, matrix);
	out->texcoord = in->texcoord;
} // vertex3d_transform

static inline void vertex3d_project_to_screen(point4d_t* out, point4d_t* v,
	i32 width, i32 height)
{
	f32 z_inv = 1.0f / v->z;
	out->x = (v->x * z_inv + 1.0f) * width / 2.0f;
	out->y = (-v->y * z_inv + 1.0f) * height / 2.0f;
	out->z = z_inv;
} // vertex3d_project_to_screen

#endif // VERTEX3D_H
