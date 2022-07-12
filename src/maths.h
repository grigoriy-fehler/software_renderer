#ifndef MATHS_H
#define MATHS_H

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long int i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef float f32;
typedef double f64;

#define PI (3.141592654f)
#define PI2 (6.283185307f)
#define PI_DIV_2 (1.570796327f)
#define PI_DIV_4 (0.785398163f)
#define PI_INV (0.318309886f)

#define EPSILON_E4 (f32)(1E-4)
#define EPSILON_E5 (f32)(1E-5)
#define EPSILON_E6 (f32)(1E-6)

#define deg_to_rad(degrees) ((degrees) * PI / 180.0)
#define rad_to_deg(radians) ((radians) * 180.0 / PI)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (b) : (a))

#define vector2d(x, y) (vector2d_t) {{ (f32) (x), (f32) (y) }}
#define vector3d(x, y, z) (vector3d_t) {{ (f32) (x), (f32) (y), (f32) (z) }}
#define vector4d(x, y, z) (vector4d_t) {{ \
	(f32) (x), \
	(f32) (y), \
	(f32) (z), \
	(f32) (0.0f) \
}}

#define point2d(x, y) (point2d_t) {{ (f32) (x), (f32) (y) }}
#define point3d(x, y, z) (point3d_t) {{ (f32) (x), (f32) (y), (f32) (z) }}
#define point4d(x, y, z) (point4d_t) {{ \
	(f32) (x), \
	(f32) (y), \
	(f32) (z), \
	(f32) (1.0f) \
}}

#define transform4d(position, rotation, scale) (transform4d_t) { \
	(point4d_t) (position), \
	(vector4d_t) (rotation), \
	(vector4d_t) (scale) \
}

#define matrix2x2(e00, e01, e10, e11) (matrix2x2_t) {{ \
	(f32) e00, (f32) e01, \
	(f32) e10, (f32) e11 \
}}
#define matrix2x2_identity (matrix2x2_t) {{ \
	1, 0, \
	0, 1 \
}}
#define matrix3x3(e00, e01, e02, e10, e11, e12, e20, e21, e22) (matrix3x3_t) {{ \
	(f32) e00, (f32) e01, (f32) e02, \
	(f32) e10, (f32) e11, (f32) e12, \
	(f32) e20, (f32) e21, (f32) e22 \
}}
#define matrix3x3_identity (matrix3x3_t) {{ \
	1, 0, 0, \
	0, 1, 0, \
	0, 0, 1 \
}}
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

typedef union vector2d_t {
	struct { f32 x, y; };
	struct { f32 u, v; };
	struct { f32 w, h; };
	struct { f32 min, max; };
	f32 e[2];
} vector2d_t, point2d_t;

typedef union vector3d_t {
	struct { f32 x, y, z; };
	struct { f32 u, v, w; };
	vector2d_t xy;
	vector2d_t uv;
	f32 e[3];
} vector3d_t, point3d_t;

typedef union vector4d_t {
	struct { f32 x, y, z, w; };
	struct { f32 top, right, bottom, left; };
	vector3d_t xyz;
	vector2d_t xy;
	f32 e[4];
} vector4d_t, point4d_t;

typedef struct transform4d_t {
	point4d_t position;
	vector4d_t rotation;
	vector4d_t scale;
} transform4d_t;

typedef union matrix2x2_t {
	struct {
		f32 e00, e01;
		f32 e10, e11;
	};
	f32 e[4];
} matrix2x2_t;

typedef union matrix3x3_t {
	struct {
		f32 e00, e01, e02;
		f32 e10, e11, e12;
		f32 e20, e21, e22;
	};
	f32 e[9];
} matrix3x3_t;

typedef union matrix4x4_t {
	struct {
		f32 e00, e01, e02, e03;
		f32 e10, e11, e12, e13;
		f32 e20, e21, e22, e23;
		f32 e30, e31, e32, e33;
	};
	f32 e[16];
} matrix4x4_t;

// G L O B A L   V A R I A B L E S /////////////////////////////////////////////

extern const f32 cos_lookup_table[361];
extern const f32 sin_lookup_table[361];
extern const f32 tan_lookup_table[361];

// M A T H   F U N C T I O N S /////////////////////////////////////////////////

static inline void swapi(i32* a, i32* b) {
	i32 temp = *a;
	*a = *b;
	*b = temp;
} // swapi

static inline void swapf(f32* a, f32* b) {
	f32 temp = *a;
	*a = *b;
	*b = temp;
} // swapf

static inline f32 absolute(f32 n) {
	return (n < 0) ? -n : n;
} // absolute

static inline i32 floor(f32 n) {
	return (i32) n;
} // floor

static inline i32 round(f32 n) {
	return (i32) (n + 0.5f);
} // round

static inline i32 ceil(f32 n) {
	return (i32) (n + 1.0f);
} // ceil

static inline f32 mod(f32 x, f32 y) {
	if (y == 0.0f) return x;

	f32 m = x - y * floor(x / y);

	if (y > 0) {
		if (m >= y) return 0;

		if (m < 0) {
			if (y + m == y) return 0;
			else return y + m;
		}
	} else {
		if (m <= y) return 0;

		if (m > 0) {
			if (y + m == y) return 0;
			else return y + m;
		}
	}

	return m;
} // mod

static inline f32 sqrt(f32 n) {
	if (n < 0) return n;
	i32 i = 0;
	f32 a = 1.0f;
	f32 b = n;
	while (absolute(a - b) > 0.000001) {
		a = (a + b) / 2;
		b = n / a;
		if (i > 10) break;
		i++;
	}
	return a;
} // sqrt

static inline f32 sin(f32 theta) {
	theta = mod(theta, 360.0f);

	i32 theta_int = (i32) theta;
	f32 theta_frac = theta - theta_int;

	return (sin_lookup_table[theta_int]  + theta_frac * (
		sin_lookup_table[theta_int + 1] - sin_lookup_table[theta_int]
	));
} // sin

static inline f32 cos(f32 theta) {
	theta = mod(theta, 360.0f);

	i32 theta_int = (i32) theta;
	f32 theta_frac = theta - theta_int;

	return (cos_lookup_table[theta_int]  + theta_frac * (
		cos_lookup_table[theta_int + 1] - cos_lookup_table[theta_int]
	));
} // cos

static inline f32 tan(f32 theta) {
	theta = mod(theta, 360.0f);

	i32 theta_int = (i32) theta;
	f32 theta_frac = theta - theta_int;

	return (tan_lookup_table[theta_int]  + theta_frac * (
		tan_lookup_table[theta_int + 1] - tan_lookup_table[theta_int]
	));
} // tan

static inline f32 clamp(f32 value, f32 min, f32 max)
{
	return (value <= min) ? min : (value >= max) ? max : value;
} // clamp

static inline f32 lerp(f32 s, f32 e, f32 t) {
	return s + (e - s) * t;
} // lerp

// V E C T O R 2   F U N C T I O N S ///////////////////////////////////////////

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

// V E C T O R 3   F U N C T I O N S ///////////////////////////////////////////

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

// V E C T O R 4   F U N C T I O N S ///////////////////////////////////////////

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

// M A T R I X 4 X 4   F U N C T I O N S ///////////////////////////////////////

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
	f32 sin_theta = sin(angle);
	f32 cos_theta = cos(angle);
	m->e11 = cos_theta;
	m->e12 = sin_theta;
	m->e21 = -sin_theta;
	m->e22 = cos_theta;
} // matrix4x4_rotation_x

// Note: Expects identity matrix for m
static inline void matrix4x4_rotation_y(matrix4x4_t* m, f32 angle) {
	f32 sin_theta = sin(angle);
	f32 cos_theta = cos(angle);
	m->e00 = cos_theta;
	m->e02 = -sin_theta;
	m->e20 = sin_theta;
	m->e22 = cos_theta;
} // matrix4x4_rotation_y

// Note: Expects identity matrix for m
static inline void matrix4x4_rotation_z(matrix4x4_t* m, f32 angle) {
	f32 sin_theta = sin(angle);
	f32 cos_theta = cos(angle);
	m->e00 = cos_theta;
	m->e01 = sin_theta;
	m->e10 = -sin_theta;
	m->e11 = cos_theta;
} // matrix4x4_rotation_z

static inline void matrix4x4_projection(matrix4x4_t* m, f32 near,
	f32 far, f32 fov, f32 width, f32 height)
{
	f32 aspect = (f32) height / (f32) width;
	f32 fov_rad = 1.0f / tan(fov * 0.5f);

	m->e00 = aspect * -fov_rad;
	m->e11 = fov_rad;
	m->e22 = far / (far - near);
	m->e23 = 1;
	m->e32 = (-far * near) / (far - near);
} // matrix4x4_perspective

#endif // MATHS_H