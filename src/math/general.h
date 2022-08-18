#ifndef GENERAL_H
#define GENERAL_H

#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define deg_to_rad(degrees) ((degrees) * PI / 180.0)
#define rad_to_deg(radians) ((radians) * 180.0 / PI)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (b) : (a))

// G L O B A L   V A R I A B L E S /////////////////////////////////////////////

extern const f32 cos_lookup_table[361];
extern const f32 sin_lookup_table[361];
extern const f32 tan_lookup_table[361];

// F U N C T I O N S ///////////////////////////////////////////////////////////

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

static inline i32 gf_floor(f32 n) {
	return (i32) n;
} // gf_floor

static inline i32 gf_round(f32 n) {
	return (i32) (n + 0.5f);
} // gf_round

static inline i32 gf_ceil(f32 n) {
	return (i32) (n + 1.0f);
} // gf_ceil

static inline f32 gf_mod(f32 x, f32 y) {
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
} // gf_mod

static inline f32 gf_sqrt(f32 n) {
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
} // gf_sqrt

static inline f32 gf_sin(f32 theta) {
	theta = gf_mod(theta, 360.0f);

	i32 theta_int = (i32) theta;
	f32 theta_frac = theta - theta_int;

	return (sin_lookup_table[theta_int]  + theta_frac * (
		sin_lookup_table[theta_int + 1] - sin_lookup_table[theta_int]
	));
} // gf_sin

static inline f32 gf_cos(f32 theta) {
	theta = gf_mod(theta, 360.0f);

	i32 theta_int = (i32) theta;
	f32 theta_frac = theta - theta_int;

	return (cos_lookup_table[theta_int]  + theta_frac * (
		cos_lookup_table[theta_int + 1] - cos_lookup_table[theta_int]
	));
} // gf_cos

static inline f32 gf_tan(f32 theta) {
	theta = gf_mod(theta, 360.0f);

	i32 theta_int = (i32) theta;
	f32 theta_frac = theta - theta_int;

	return (tan_lookup_table[theta_int]  + theta_frac * (
		tan_lookup_table[theta_int + 1] - tan_lookup_table[theta_int]
	));
} // gf_tan

static inline f32 clamp(f32 value, f32 min, f32 max)
{
	return (value <= min) ? min : (value >= max) ? max : value;
} // clamp

static inline f32 lerp(f32 s, f32 e, f32 t) {
	return s + (e - s) * t;
} // lerp

static inline i32 equal(f32 a, f32 b, f32 tolerance) {
	if (absolute(a - b) > tolerance) return 0;
	return 1;
} // equal

#endif // GENERAL_H
