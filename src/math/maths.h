#ifndef MATHS_H
#define MATHS_H

#include <stdint.h>

#ifndef PI
    #define PI 3.14159265358979323846
#endif

float deg2rad(uint32_t degrees);
int32_t rad2deg(float radians);

int32_t clamp(int32_t value, int32_t min, int32_t max);
int32_t wrap(int32_t value, int32_t min, int32_t max);
int32_t lerp(int32_t start, int32_t end, int32_t amount);
uint32_t normalize(int32_t value, int32_t start, int32_t end, uint32_t );

float clampf(float value, float min, float max);
float wrapf(float value, float min, float max);
float lerpf(float start, float end, float amount);
float normalizef(float value, float start, float end);

#endif
