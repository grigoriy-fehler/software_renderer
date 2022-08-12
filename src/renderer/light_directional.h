#ifndef LIGHT_DIRECTIONAL_H
#define LIGHT_DIRECTIONAL_H

#include "../math/mathlib.h"

#include "color_rgba.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct directional_light_t {
	vector4d_t direction;
	color_rgba_t diffuse;
} directional_light_t;

#endif // LIGHT_DIRECTIONAL_H
