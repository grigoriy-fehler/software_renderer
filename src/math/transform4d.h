#ifndef TRANSFORM4D_H
#define TRANSFORM4D_H

#include "vector4d.h"
#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define transform4d(position, rotation, scale) (transform4d_t) { \
	(point4d_t) (position), \
	(vector4d_t) (rotation), \
	(vector4d_t) (scale) \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct transform4d_t {
	point4d_t position;
	vector4d_t rotation;
	vector4d_t scale;
} transform4d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////



#endif // TRANSFORM4D_H
