#ifndef VECTOR4D_H
#define VECTOR4D_H

#include "matrix4x4.h"
#include "general.h"
#include "types.h"
#include "vector2d.h"
#include "vector3d.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define vector4d(x, y, z) (vector4d_t) {{ \
	(f32) (x), \
	(f32) (y), \
	(f32) (z), \
	(f32) (0.0f) \
}}

#define point4d(x, y, z) (point4d_t) {{ \
	(f32) (x), \
	(f32) (y), \
	(f32) (z), \
	(f32) (1.0f) \
}}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union vector4d_t {
	struct { f32 x, y, z, w; };
	struct { f32 top, right, bottom, left; };
	vector3d_t xyz;
	vector2d_t xy;
	f32 e[4];
} vector4d_t, point4d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void vector4d_add(vector4d_t* out, vector4d_t* a, vector4d_t* b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
} // vector4d_add

static inline void vector4d_add_float(vector4d_t* out, vector4d_t* v, f32 f) {
	out->x = v->x + f;
	out->y = v->y + f;
	out->z = v->z + f;
	out->w = v->w + f;
} // vector4d_add_float

static inline void vector4d_subtract(vector4d_t* out, vector4d_t* a,
	vector4d_t* b)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->w = a->w - b->w;
} // vector4d_subtract

static inline void vector4d_subtract_float(vector4d_t* out, vector4d_t* v,
	f32 f)
{
	out->x = v->x - f;
	out->y = v->y - f;
	out->z = v->z - f;
	out->w = v->w - f;
} // vector4d_subtract_float

static inline void vector4d_multiply(vector4d_t* out, vector4d_t* a,
	vector4d_t* b)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
	out->w = a->w * b->w;
} // vector4d_multiply

static inline void vector4d_multiply_float(vector4d_t* out, vector4d_t* v,
	f32 f)
{
	out->x = v->x * f;
	out->y = v->y * f;
	out->z = v->z * f;
	out->w = v->w * f;
} // vector4d_multiply_float

static inline void vector4d_divide(vector4d_t* out, vector4d_t* a,
	vector4d_t* b)
{
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
	out->w = a->w / b->w;
} // vector4d_divide

static inline void vector4d_divide_float(vector4d_t* out, vector4d_t* v,
	f32 f)
{
	f32 f_inv = 1.0f / f;
	out->x = v->x * f_inv;
	out->y = v->y * f_inv;
	out->z = v->z * f_inv;
	out->w = v->w * f_inv;
} // vector4d_divide_float

static inline f32 vector4d_length(vector4d_t* v) {
	return sqrt(
		(v->x * v->x) +
		(v->y * v->y) +
		(v->z * v->z) +
		(v->w * v->w)
	);
} // vector4d_length

static inline f32 vector4d_length_sqr(vector4d_t* v) {
	return (v->x * v->x) + (v->y * v->y) + (v->z * v->z) + (v->w * v->w);
} // vector4d_length_sqr

static inline void vector4d_normalize(vector4d_t* out, vector4d_t* v) {
	f32 len = vector4d_length(v);
	if (len == 0.0f) len = 1.0f;
	f32 len_inv = 1.0f / len;

	return vector4d_multiply_float(out, v, len_inv);
} // vector4d_normalize

static inline void vector4d_negate(vector4d_t* out, vector4d_t* v) {
	out->x = -v->x;
	out->y = -v->y;
	out->z = -v->z;
	out->w = -v->w;
} // vector4d_negate

static inline void vector4d_lerp(vector4d_t* out, vector4d_t* a, vector4d_t* b,
	f32 t)
{
	out->x = lerp(a->x, b->x, t);
	out->y = lerp(a->y, b->y, t);
	out->z = lerp(a->z, b->z, t);
	out->w = lerp(a->w, b->w, t);
} // vector4d_lerp

static inline void vector4d_clamp(vector4d_t* out, vector4d_t* v, f32 min,
	f32 max)
{
	out->x = clamp(v->x, min, max);
	out->y = clamp(v->y, min, max);
	out->z = clamp(v->z, min, max);
	out->w = clamp(v->w, min, max);
} // vector4d_clamp

static inline void vector4d_multiply_matrix4x4(vector4d_t* out, vector4d_t* v,
	matrix4x4_t* m)
{
	for (i32 col = 0; col < 4; col++) {
		f32 sum = 0;
		for (i32 row = 0; row < 4; row++) {
			sum += (v->e[row] * m->e[row * 4 + col]);
		}
		out->e[col] = sum;
	}
} // vector4d_multiply_matrix4x4

#endif // VECTOR4D_H
