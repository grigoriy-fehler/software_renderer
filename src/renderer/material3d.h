#ifndef MATERIAL3D_H
#define MATERIAL3D_H

#include "../math/mathlib.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define material3d(texture_index) (material3d_t) { (u32) (texture_index) }

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct material3d_t {
	u32 texture_index;
} material3d_t;

#endif // MATERIAL3D_H
