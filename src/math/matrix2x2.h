#ifndef MATRIX2X2_H
#define MATRIX2X2_H

#include "types.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define matrix2x2(e00, e01, e10, e11) (matrix2x2_t) {{ \
	(f32) e00, (f32) e01, \
	(f32) e10, (f32) e11 \
}}
#define matrix2x2_identity (matrix2x2_t) {{ \
	1, 0, \
	0, 1 \
}}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef union matrix2x2_t {
	struct {
		f32 e00, e01;
		f32 e10, e11;
	};
	f32 e[4];
} matrix2x2_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////



#endif // MATRIX2X2_H
