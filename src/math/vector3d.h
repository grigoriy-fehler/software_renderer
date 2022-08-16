#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "general.h"
#include "types.h"
#include "vector2d.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define vector3d(x, y, z) (vector3d_t) {{ (f32) (x), (f32) (y), (f32) (z) }}

#define point3d(x, y, z) (point3d_t) {{ (f32) (x), (f32) (y), (f32) (z) }}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union vector3d_t {
	struct { f32 x, y, z; };
	struct { f32 u, v, w; };
	vector2d_t xy;
	vector2d_t uv;
	f32 e[3];
} vector3d_t, point3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void vector3d_add(vector3d_t* out, vector3d_t* a, vector3d_t* b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
} // vector3d_add

static inline void vector3d_add_float(vector3d_t* out, vector3d_t* v, f32 f) {
	out->x = v->x + f;
	out->y = v->y + f;
	out->z = v->z + f;
} // vector3d_add_float

static inline void vector3d_subtract(vector3d_t* out, vector3d_t* a,
	vector3d_t* b)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
} // vector3d_subtract

static inline void vector3d_subtract_float(vector3d_t* out, vector3d_t* v,
	f32 f)
{
	out->x = v->x - f;
	out->y = v->y - f;
	out->z = v->z - f;
} // vector3d_subtract_float

static inline void vector3d_multiply(vector3d_t* out, vector3d_t* a,
	vector3d_t* b)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
} // vector3d_multiply

static inline void vector3d_multiply_float(vector3d_t* out, vector3d_t* v,
	f32 f)
{
	out->x = v->x * f;
	out->y = v->y * f;
	out->z = v->z * f;
} // vector3d_multiply_float

static inline void vector3d_divide(vector3d_t* out, vector3d_t* a,
	vector3d_t* b)
{
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
} // vector3d_divide

static inline void vector3d_divide_float(vector3d_t* out, vector3d_t* v, f32 f)
{
	f32 f_inv = 1.0f / f;
	out->x = v->x * f_inv;
	out->y = v->y * f_inv;
	out->z = v->z * f_inv;
} // vector3d_divide_float

static inline f32 vector3d_length(vector3d_t* v) {
	return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
} // vector3d_length

static inline f32 vector3d_length_sqr(vector3d_t* v) {
	return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
} // vector3d_length_sqr

static inline void vector3d_normalize(vector3d_t* out, vector3d_t* v) {
	f32 len = vector3d_length(v);
	if (len == 0.0f) len = 1.0f;

	f32 len_inv = 1.0f / len;

	vector3d_multiply_float(out, v, len_inv);
} // vector3d_normalize

static inline void vector3d_negate(vector3d_t* out, vector3d_t* v) {
	out->x = -v->x;
	out->y = -v->y;
	out->z = -v->z;
} // vector3d_negate

static inline i32 vector3d_equal(vector3d_t* a, vector3d_t* b, f32 tolerance) {
	if (absolute(a->x - b->x) > tolerance) return 0;
	if (absolute(a->y - b->y) > tolerance) return 0;
	if (absolute(a->z - b->z) > tolerance) return 0;
	return 1;
} // vector3d_equal

static inline void vector3d_clamp(vector3d_t* out, vector3d_t* v, f32 min,
	f32 max)
{
	out->x = clamp(v->x, min, max);
	out->y = clamp(v->y, min, max);
	out->z = clamp(v->z, min, max);
} // vector3d_clamp

static inline void vector3d_cross_product(vector3d_t* out, vector3d_t* a,
	vector3d_t* b)
{
	out->x = (a->y * b->z - a->z * b->y);
	out->y = (a->z * b->x - a->x * b->z);
	out->z = (a->x * b->y - a->y * b->x);
} // vector3d_cross_product

static inline f32 vector3d_dot_product(vector3d_t* a, vector3d_t* b) {
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
} // vector3d_dot_product

static inline f32 vector3d_distance(vector3d_t* a, vector3d_t* b) {
	vector3d_t v = vector3d(
		a->x - b->x,
		a->y - b->y,
		a->z - b->z
	);
	return vector3d_length(&v);
} // vector3d_distance

static inline void vector3d_lerp(vector3d_t* out, vector3d_t* a, vector3d_t* b,
	f32 t)
{
	out->x = lerp(a->x, b->x, t);
	out->y = lerp(a->y, b->y, t);
	out->z = lerp(a->z, b->z, t);
} // vector3d_lerp

static inline void vector3d_swap(vector3d_t* a, vector3d_t* b) {
	swapf(&a->x, &b->x);
	swapf(&a->y, &b->y);
	swapf(&a->z, &b->z);
} // vector3d_swap

#endif // VECTOR3D_H
