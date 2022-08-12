#ifndef POLYGON3D_H
#define POLYGON3D_H

#include "../math/mathlib.h"

#include "vertex3d.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define polygon3d(vertex_count, vertices) (polygon3d_t) { \
	(u32) (vertex_count), \
	(vertex3d_t*) (vertices) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct polygon3d_t {
	u32 vertex_count;
	vertex3d_t* vertices;
} polygon3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void polygon3d_transform(polygon3d_t* out, polygon3d_t* in,
	matrix4x4_t* matrix)
{
	for (u32 i = 0; i < in->vertex_count; i++) {
		vertex3d_transform(&out->vertices[i], &in->vertices[i], matrix);
	}
} // polygon3d_transform

static inline void polygon3d_project_to_screen(polygon3d_t* out,
	polygon3d_t* in, i32 width, i32 height)
{
	for (u32 i = 0; i < in->vertex_count; i++) {
		vertex3d_project_to_screen(
			&out->vertices[i].position,
			&in->vertices[i].position,
			width,
			height
		);
		out->vertices[i].texcoord = in->vertices[i].texcoord;
	}
} // polygon3d_project_to_screen

static inline u8 polygon3d_cull(polygon3d_t* poly, point3d_t* cam_pos) {
	point3d_t p1 = poly->vertices[0].position.xyz;
	point3d_t p2 = poly->vertices[1].position.xyz;
	point3d_t p3 = poly->vertices[2].position.xyz;

	vector3d_t l1;
	vector3d_subtract(&l1, &p1, &p2);
	vector3d_t l2;
	vector3d_subtract(&l2, &p1, &p3);
	vector3d_t n;
	vector3d_cross_product(&n, &l1, &l2);
	vector3d_t n_normalized;
	vector3d_normalize(&n_normalized, &n);
	vector3d_t p;
	vector3d_subtract(&p, &p1, cam_pos);

	f32 dot_product = vector3d_dot_product(
		&n_normalized,
		&p
	);

	if (dot_product > 0.0f) return 1;
	return 0;
} // polygon3d_cull

#endif // POLYGON3D_H
