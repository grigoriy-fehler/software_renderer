#ifndef PLANE3D_H
#define PLANE3D_H

#include "vector3d.h"
#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define plane3d(distance, normal) (plane3d_t) { \
	(f32) (distance), \
	(vector3d_t) (normal) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct plane3d_t {
	f32 distance;
	vector3d_t normal;
} plane3d_t;

#endif // PLANE3D_H
