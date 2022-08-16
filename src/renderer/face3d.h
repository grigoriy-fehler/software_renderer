#ifndef FACE3D_H
#define FACE3D_H

#include "../math/mathlib.h"

#include "index3d.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define face3d(index_count, indices) (face3d_t) { \
	(u32) (index_count), \
	(index3d_t *) (indices) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct face3d_t {
	u32 index_count;
	index3d_t* indices;
} face3d_t;

#endif // FACE3D_H
