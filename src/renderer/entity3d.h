#ifndef RENDER_ENTITY3D_H
#define RENDER_ENTITY3D_H

#include "../math/mathlib.h"

#include "camera.h"
#include "framebuffer.h"
#include "triangle3d.h"
#include "vertex3d.h"
#include "face3d.h"
#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct render_entity3d_t {
	u32 vertex_count;
	point4d_t* vertices;
	u32 texcoord_count;
	point2d_t* texcoords;
	u32 normal_count;
	vector4d_t* normals;
	u32 face_count;
	face3d_t* faces;
	u32 material_index;
	transform4d_t transform;
} render_entity3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void render_entity3d_create_rotation_matrix(matrix4x4_t* out,
	vector4d_t* rotation)
{
	matrix4x4_t x = matrix4x4_identity;
	matrix4x4_t y = matrix4x4_identity;
	matrix4x4_t z = matrix4x4_identity;
	matrix4x4_t temp = matrix4x4_identity;

	matrix4x4_rotation_y(&y, rotation->y);
	matrix4x4_rotation_x(&x, rotation->x);
	matrix4x4_rotation_z(&z, rotation->z);

	matrix4x4_multiply(&temp, &y, &x);
	matrix4x4_multiply(out, &temp, &z);
} // render_entity3d_create_rotation_matrix

static inline void render_entity3d_create_scale_matrix(matrix4x4_t* out,
	vector4d_t* scale)
{
	matrix4x4_scale(
		out,
		scale->x,
		scale->y,
		scale->z
	);
} // render_entity3d_create_scale_matrix

static inline void render_entity3d_create_inverse_scale_matrix(matrix4x4_t* out,
	vector4d_t* scale)
{
	matrix4x4_scale(
		out,
		1.0f / scale->x,
		1.0f / scale->y,
		1.0f / scale->z
	);
} // render_entity3d_create_inverse_scale_matrix

#endif // RENDER_ENTITY3D_H
