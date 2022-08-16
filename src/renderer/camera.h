#ifndef CAMERA_H
#define CAMERA_H

#include "../math/mathlib.h"

#include "framebuffer.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define CLIPPING_PLANES_COUNT 6

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct camera_t {
	point4d_t position;
	vector4d_t direction;
	f32 fov;
	f32 z_far;
	f32 z_near;
	plane3d_t clipping_planes[CLIPPING_PLANES_COUNT];
	matrix4x4_t matrix;
} camera_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void camera_create_euler_matrix(matrix4x4_t* m, camera_t* c) {
	f32 x = c->direction.x;
	f32 y = c->direction.y;
	f32 z = c->direction.z;

	matrix4x4_t mt = matrix4x4_identity;
	mt.e30 = -c->position.x;
	mt.e31 = -c->position.y;
	mt.e32 = -c->position.z;

	f32 cos_x = gf_cos(x);
	f32 sin_x = -gf_sin(x);
	matrix4x4_t mrx = matrix4x4_identity;
	mrx.e11 = cos_x;
	mrx.e12 = sin_x;
	mrx.e21 = -sin_x;
	mrx.e22 = cos_x;

	f32 cos_y = gf_cos(y);
	f32 sin_y = -gf_sin(y);
	matrix4x4_t mry = matrix4x4_identity;
	mry.e00 = cos_y;
	mry.e02 = -sin_y;
	mry.e20 = sin_y;
	mry.e22 = cos_y;

	f32 cos_z = gf_cos(z);
	f32 sin_z = -gf_sin(z);
	matrix4x4_t mrz = matrix4x4_identity;
	mrz.e00 = cos_z;
	mrz.e01 = sin_z;
	mrz.e10 = -sin_z;
	mrz.e11 = cos_z;

	matrix4x4_t mr = matrix4x4_identity;
	matrix4x4_t mtemp = matrix4x4_identity;
	matrix4x4_multiply(&mtemp, &mry, &mrx);
	matrix4x4_multiply(&mr, &mtemp, &mrz);
	matrix4x4_multiply(m, &mt, &mr);
} // camera_create_euler_matrix

// TODO: Fix clipping
static inline void camera_create_clipping_planes(camera_t* camera,
	framebuffer_t* fb)
{
	f32 clip_dir_x, clip_dir_y, clip_dir_z;
	f32 aspect = (f32) fb->height / (f32) fb->width;
	f32 half_fov = camera->fov * 0.5f;
	f32 half_fov_aspect = half_fov * aspect;

	// Near Plane
	camera->clipping_planes[0] = plane3d(
		camera->z_near,
		vector3d(0.0f, 0.0f, 1.0f)
	);
	// Far Plane
	camera->clipping_planes[1] = plane3d(
		-camera->z_far,
		vector3d(0.0f, 0.0f, -1.0f)
	);
	// Left Plane
	clip_dir_x = 0.0f - gf_sin(half_fov_aspect);
	clip_dir_z = 1.0f + gf_cos(half_fov_aspect);
	vector3d_t clip_dir = vector3d(clip_dir_x, 0.0f, clip_dir_z);
	vector3d_normalize(&clip_dir, &clip_dir);
	camera->clipping_planes[2] = plane3d(
		0,
		clip_dir
	);
	// Right Plane
	clip_dir_x = 0.0f + gf_sin(half_fov_aspect);
	clip_dir_z = 1.0f + gf_cos(half_fov_aspect);
	clip_dir = vector3d(clip_dir_x, 0.0f, clip_dir_z);
	vector3d_normalize(&clip_dir, &clip_dir);
	camera->clipping_planes[3] = plane3d(
		0,
		clip_dir
	);
	// Top Plane
	clip_dir_y = 0.0f - gf_sin(half_fov);
	clip_dir_z = 1.0f + gf_cos(half_fov);
	clip_dir = vector3d(0.0f, clip_dir_y, clip_dir_z);
	vector3d_normalize(&clip_dir, &clip_dir);
	camera->clipping_planes[4] = plane3d(
		0,
		clip_dir
	);
	// Bottom Plane
	clip_dir_y = 0.0f + gf_sin(half_fov);
	clip_dir_z = 1.0f + gf_cos(half_fov);
	clip_dir = vector3d(0.0f, clip_dir_y, clip_dir_z);
	vector3d_normalize(&clip_dir, &clip_dir);
	camera->clipping_planes[5] = plane3d(
		0,
		clip_dir
	);
} // camera_create_clipping_planes

#endif // CAMERA_H
