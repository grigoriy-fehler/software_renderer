#ifndef TYPES_H
#define TYPES_H

#ifdef NO_STD
	typedef signed char i8;
	typedef signed short i16;
	typedef signed int i32;
	typedef signed long long int i64;

	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned int u32;
	typedef unsigned long long int u64;
#else
	#include <stdint.h>

	typedef int8_t i8;
	typedef int16_t i16;
	typedef int32_t i32;
	typedef int64_t i64;

	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;
#endif

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

#endif // TYPES_H
