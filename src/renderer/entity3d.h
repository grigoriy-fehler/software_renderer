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
	u32 face_count;
	face3d_t* faces;
	u32 material_index;
	transform4d_t transform;
} render_entity3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void create_rotation_matrix(matrix4x4_t* out,
	vector4d_t* rotation)
{
	matrix4x4_t mrx = matrix4x4_identity;
	matrix4x4_t mry = matrix4x4_identity;
	matrix4x4_t mrz = matrix4x4_identity;
	matrix4x4_t temp = matrix4x4_identity;

	matrix4x4_rotation_y(&mry, rotation->y);
	matrix4x4_rotation_x(&mrx, rotation->x);
	matrix4x4_rotation_z(&mrz, rotation->z);

	matrix4x4_multiply(&temp, &mry, &mrx);
	matrix4x4_multiply(out, &temp, &mrz);
} // create_rotation_matrix

static inline void create_entity_matrix(matrix4x4_t* out,
	render_entity3d_t* entity)
{
	matrix4x4_t entity_rotation_matrix = matrix4x4_identity;
	create_rotation_matrix(
		&entity_rotation_matrix,
		&entity->transform.rotation
	);
	matrix4x4_t entity_scale_matrix = matrix4x4_identity;
	matrix4x4_scale(
		&entity_scale_matrix,
		entity->transform.scale.x,
		entity->transform.scale.y,
		entity->transform.scale.z
	);
	matrix4x4_t entity_translation_matrix = matrix4x4_identity;
	matrix4x4_translation(
		&entity_translation_matrix,
		entity->transform.position.x,
		entity->transform.position.y,
		entity->transform.position.z
	);
	matrix4x4_t entity_matrix_rs = matrix4x4_identity;
	matrix4x4_multiply(
		&entity_matrix_rs,
		&entity_scale_matrix,
		&entity_rotation_matrix
	);
	matrix4x4_multiply(
		out,
		&entity_matrix_rs,
		&entity_translation_matrix
	);
} // create_entity_matrix

#endif // RENDER_ENTITY3D_H
