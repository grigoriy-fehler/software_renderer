#include <math.h>

#include "maths.h"

float deg2rad(uint32_t degrees) {
    return degrees * (PI/180.0f);
}

int32_t rad2deg(float radians) {
    return (int32_t) radians * (180.0f / PI);
}

int32_t clamp(int32_t value, int32_t min, int32_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int32_t wrap(int32_t value, int32_t min, int32_t max) {
    if (value < min) return max;
    if (value > max) return min;
    return value;
}

int32_t lerp(int32_t start, int32_t end, int32_t amount) {
    return start + amount * (end - start);
}

uint32_t normalize(int32_t value, int32_t start, int32_t end, uint32_t size) {
	float temp = (float) (value - start) / (end - start);
    return (uint32_t) temp * size;
}

float clampf(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float wrapf(float value, float min, float max) {
    if (value < min) return max;
    if (value > max) return min;
    return value;
}

float lerpf(float start, float end, float amount) {
    return start + amount * (end - start);
}

float normalizef(float value, float start, float end) {
    return (value - start) / (end - start);
}
