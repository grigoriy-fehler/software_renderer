#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "../math/mathlib.h"

#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define vertex3d(position, texcoord, normal, color) (vertex3d_t) { \
	(point4d_t) (position), \
	(point2d_t) (texcoord), \
	(vector4d_t) (normal), \
	(color_rgba_t) (color) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct vertex3d_t {
	point4d_t position;
	point2d_t texcoord;
	vector4d_t normal;
	color_rgba_t color;
} vertex3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void vertex3d_swap(vertex3d_t* a, vertex3d_t* b) {
	vector4d_swap(&a->position, &b->position);
	vector2d_swap(&a->texcoord, &b->texcoord);
	vector4d_swap(&a->normal, &b->normal);
	vector4d_swap(&a->color.rgba, &b->color.rgba);
} // vertex3d_swap

static inline void vertex3d_lerp(vertex3d_t* out, vertex3d_t* a, vertex3d_t* b,
	f32 t)
{
	vector4d_lerp(&out->position, &a->position, &b->position, t);
	vector2d_lerp(&out->texcoord, &a->texcoord, &b->texcoord, t);
	vector4d_lerp(&out->normal, &a->normal, &b->normal, t);
	vector4d_lerp(&out->color.rgba, &a->color.rgba, &b->color.rgba, t);
} // vertex3d_lerp

static inline void vertex3d_transform(vertex3d_t* out, vertex3d_t* in,
	matrix4x4_t* matrix)
{
	vector4d_multiply_matrix4x4(&out->position, &in->position, matrix);
	out->texcoord = in->texcoord;
	vector4d_multiply_matrix4x4(
		&out->normal,
		&in->normal,
		matrix
	);
	vector3d_normalize(&out->normal.xyz, &out->normal.xyz);
	out->color = in->color;
} // vertex3d_transform

static inline void vertex3d_translate(vertex3d_t* out, vertex3d_t* in,
	vector4d_t* trans)
{
	vector4d_add(&out->position, &in->position, trans);
	out->texcoord = in->texcoord;
	out->normal.x = in->normal.x + trans->x;
	out->normal.y = in->normal.y + trans->y;
	out->normal.z = in->normal.z + trans->z;
	vector3d_normalize(&out->normal.xyz, &out->normal.xyz);
	out->color = in->color;
} // vertex3d_translate

static inline void vertex3d_project_to_screen(point4d_t* out, point4d_t* v,
	i32 width, i32 height)
{
	f32 z_inv = 1.0f / v->z;
	out->x = (v->x * z_inv + 1.0f) * width / 2.0f;
	out->y = (-v->y * z_inv + 1.0f) * height / 2.0f;
	out->z = z_inv;
} // vertex3d_project_to_screen

#endif // VERTEX3D_H
