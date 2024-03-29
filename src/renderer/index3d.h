#ifndef INDEX3D_H
#define INDEX3D_H

#include "../math/mathlib.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define index3d(position, texcoord, normal) (index3d_t) { \
	(i32) (position), \
	(i32) (texcoord), \
	(i32) (normal) \
}

// D E F I N E S ///////////////////////////////////////////////////////////////

typedef struct index3d_t {
	i32 position;
	i32 texcoord;
	i32 normal;
} index3d_t;

#endif // INDEX3D_H
