#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "general.h"
#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define vector2d(x, y) (vector2d_t) {{ (f32) (x), (f32) (y) }}

#define point2d(x, y) (point2d_t) {{ (f32) (x), (f32) (y) }}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union vector2d_t {
	struct { f32 x, y; };
	struct { f32 u, v; };
	struct { f32 w, h; };
	struct { f32 min, max; };
	f32 e[2];
} vector2d_t, point2d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void vector2d_add(vector2d_t* out, vector2d_t* a, vector2d_t* b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
} // vector2d_add

static inline void vector2d_add_float(vector2d_t* out, vector2d_t* v, f32 f) {
	out->x = v->x + f;
	out->y = v->y + f;
} // vector2d_add_float

static inline void vector2d_subtract(vector2d_t* out, vector2d_t* a,
	vector2d_t* b)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
} // vector2d_subtract

static inline void vector2d_subtract_float(vector2d_t* out, vector2d_t* v,
	f32 f)
{
	out->x = v->x - f;
	out->y = v->y - f;
} // vector2d_subtract_float

static inline void vector2d_multiply(vector2d_t* out, vector2d_t* a,
	vector2d_t* b)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
} // vector2d_multiply

static inline void vector2d_multiply_float(vector2d_t* out, vector2d_t* v,
	f32 f)
{
	out->x = v->x * f;
	out->y = v->y * f;
} // vector2d_multiply_float

static inline void vector2d_divide(vector2d_t* out, vector2d_t* a,
	vector2d_t* b)
{
	out->x = a->x / b->x;
	out->y = a->y / b->y;
} // vector2d_divide

static inline void vector2d_divide_float(vector2d_t* out, vector2d_t* v, f32 f)
{
	f32 f_inv = 1.0f / f;
	out->x = v->x * f_inv;
	out->y = v->y * f_inv;
} // vector2d_divide_float

static inline f32 vector2d_length(vector2d_t* v) {
	return sqrt((v->x * v->x) + (v->y * v->y));
} // vector2d_length

static inline void vector2d_normalize(vector2d_t* out, vector2d_t* in) {
	f32 length = vector2d_length(in);
	if (length == 0.0f) length = 1.0f;

	f32 length_inv = 1.0f / length;

	vector2d_multiply_float(out, in, length_inv);
} // vector2d_normalize

static inline void vector2d_negate(vector2d_t* v) {
	v->x = -v->x;
	v->y = -v->y;
} // vector2d_negate

static inline i32 vector2d_equal(vector2d_t* a, vector2d_t* b, f32 tolerance) {
	if (absolute(a->x - b->x) > tolerance) return 0;
	if (absolute(a->y - b->y) > tolerance) return 0;
	return 1;
} // vector2d_equal

static inline void vector2d_clamp(vector2d_t* out, vector2d_t* in, f32 min,
	f32 max)
{
	out->x = clamp(in->x, min, max);
	out->y = clamp(in->y, min, max);
} // vector2d_clamp

static inline void vector2d_lerp(vector2d_t* out, vector2d_t* a, vector2d_t* b,
	f32 t)
{
	out->x = lerp(a->x, b->x, t);
	out->y = lerp(a->y, b->y, t);
} // vector2d_lerp

static inline void vector2d_swap(vector2d_t* a, vector2d_t* b) {
	swapf(&a->x, &b->x);
	swapf(&a->y, &b->y);
} // vector2d_swap

static inline float vector2d_dot_product(vector2d_t* a, vector2d_t* b) {
	return (a->x * b->x) + (a->y * b->y);
} // vector2d_dot_product

#endif // VECTOR2D_H
