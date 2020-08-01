#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color;

#define WHITE (Color){ 255, 255, 255, 255 }
#define BLACK (Color){ 0, 0, 0, 255 }
#define RED (Color){ 255, 0, 0, 255 }
#define GREEN (Color){ 0, 255, 0, 255 }
#define BLUE (Color){ 0, 0, 255, 255 }

uint32_t color_to_uint(Color c);

#endif
