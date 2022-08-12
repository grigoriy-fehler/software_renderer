#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

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

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union matrix3x3_t {
	struct {
		f32 e00, e01, e02;
		f32 e10, e11, e12;
		f32 e20, e21, e22;
	};
	f32 e[9];
} matrix3x3_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////



#endif // MATRIX3X3_H
