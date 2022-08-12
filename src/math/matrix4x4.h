#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "general.h"
#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define matrix4x4(e00, e01, e02, e03, e10, e11, e12, e13, \
	e20, e21, e22, e23, e30, e31, e32, e33) (matrix4x4_t) \
{{ \
	(f32) e00, (f32) e01, (f32) e02, (f32) e03, \
	(f32) e10, (f32) e11, (f32) e12, (f32) e13, \
	(f32) e20, (f32) e21, (f32) e22, (f32) e23, \
	(f32) e30, (f32) e31, (f32) e32, (f32) e33 \
}}
#define matrix4x4_identity (matrix4x4_t) {{ \
	1, 0, 0, 0, \
	0, 1, 0, 0, \
	0, 0, 1, 0, \
	0, 0, 0, 1 \
}}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union matrix4x4_t {
	struct {
		f32 e00, e01, e02, e03;
		f32 e10, e11, e12, e13;
		f32 e20, e21, e22, e23;
		f32 e30, e31, e32, e33;
	};
	f32 e[16];
} matrix4x4_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void matrix4x4_multiply(matrix4x4_t* out,
	matrix4x4_t* a, matrix4x4_t* b)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sum = 0;
			for (int k = 0; k < 4; k++)
				sum += *(((float *) a) + (i * 4 + k)) *
					*(((float *) b) + (k * 4 + j));
			*(((float *) out) + (i * 4 + j)) = sum;
		}
	}
} // matrix4x4_multiply

// Note: Expects identity matrix for m
static inline void matrix4x4_translation(matrix4x4_t* m, f32 x, f32 y, f32 z) {
	m->e30 = x;
	m->e31 = y;
	m->e32 = z;
} // matrix4x4_translation

// Note: Expects identity matrix for m
static inline void matrix4x4_scale(matrix4x4_t* m, f32 x, f32 y, f32 z) {
	m->e00 = x;
	m->e11 = y;
	m->e22 = z;
} // matrix4x4_scale

// Note: Expects identity matrix for m
static inline void matrix4x4_rotation_x(matrix4x4_t* m, f32 angle) {
	f32 sin_theta = gf_sin(angle);
	f32 cos_theta = gf_cos(angle);
	m->e11 = cos_theta;
	m->e12 = sin_theta;
	m->e21 = -sin_theta;
	m->e22 = cos_theta;
} // matrix4x4_rotation_x

// Note: Expects identity matrix for m
static inline void matrix4x4_rotation_y(matrix4x4_t* m, f32 angle) {
	f32 sin_theta = gf_sin(angle);
	f32 cos_theta = gf_cos(angle);
	m->e00 = cos_theta;
	m->e02 = -sin_theta;
	m->e20 = sin_theta;
	m->e22 = cos_theta;
} // matrix4x4_rotation_y

// Note: Expects identity matrix for m
static inline void matrix4x4_rotation_z(matrix4x4_t* m, f32 angle) {
	f32 sin_theta = gf_sin(angle);
	f32 cos_theta = gf_cos(angle);
	m->e00 = cos_theta;
	m->e01 = sin_theta;
	m->e10 = -sin_theta;
	m->e11 = cos_theta;
} // matrix4x4_rotation_z

static inline void matrix4x4_projection(matrix4x4_t* m, f32 near,
	f32 far, f32 fov, f32 width, f32 height)
{
	f32 aspect = (f32) height / (f32) width;
	f32 fov_rad = 1.0f / gf_tan(fov * 0.5f);

	m->e00 = aspect * -fov_rad;
	m->e11 = fov_rad;
	m->e22 = far / (far - near);
	m->e23 = 1;
	m->e32 = (-far * near) / (far - near);
} // matrix4x4_perspective

#endif // MATRIX4X4_H
