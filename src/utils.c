#include "utils.h"

void swap(int32_t* a, int32_t* b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void swapf(float* a, float* b) {
	float t = *a;
	*a = *b;
	*b = t;
}
