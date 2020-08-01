#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.h"

typedef struct Vector2 {
	int x;
	int y;
} Vector2;

typedef struct Vector2f {
	float x;
	float y;	
} Vector2f;

typedef struct {
	float x;
	float y;
	float z;
} Vector3f;

typedef struct Vector4f {
	float x;
	float y;
	float z;
	float w;
} Vector4f;

void vec2_print(Vector2 v);
Vector2 vec2_set(int x, int y);
Vector2 vec2_add(Vector2 v0, Vector2 v1);
Vector2 vec2_addi(Vector2 v, int i);
Vector2 vec2_sub(Vector2 v0, Vector2 v1);
Vector2 vec2_subi(Vector2 v, int i);
Vector2 vec2_mul(Vector2 v0, Vector2 v1);
Vector2 vec2_muli(Vector2 v, int i);
Vector2 vec2_div(Vector2 v0, Vector2 v1);
Vector2 vec2_divi(Vector2 v, int i);
Vector2 vec2_neg(Vector2 v);
float vec2_dist(Vector2 v0, Vector2 v1);
float vec2_len(Vector2 v);
int vec2_len_sqr(Vector2 v);
int vec2_dot(Vector2 v0, Vector2 v1);
int vec2_angl(Vector2 v0, Vector2 v1);
int vec2_interpolate(Vector2 v0, Vector2 v1, int y);

void vec2f_print(Vector2f v);
Vector2f vec2f_set(float x, float y);
Vector2f vec2f_add(Vector2f v0, Vector2f v1);
Vector2f vec2f_addi(Vector2f v, float f);
Vector2f vec2f_sub(Vector2f v0, Vector2f v1);
Vector2f vec2f_subf(Vector2f v, float f);
Vector2f vec2f_mul(Vector2f v0, Vector2f v1);
Vector2f vec2f_mulf(Vector2f v, float f);
Vector2f vec2f_div(Vector2f v0, Vector2f v1);
Vector2f vec2f_divf(Vector2f v, float f);
Vector2f vec2f_neg(Vector2f v);
float vec2f_len(Vector2f v);
float vec2f_len_sqr(Vector2f v);
float vec2f_dot(Vector2f v0, Vector2f v1);
float vec2f_dist(Vector2f v0, Vector2f v1);
float vec2f_angl(Vector2f v0, Vector2f v1);
float vec2f_interpolate(Vector2f v0, Vector2f v1, float y);

void vec3f_print(Vector3f v);
Vector3f vec3f_set(float x, float y, float z);
Vector3f vec3f_add(Vector3f v0, Vector3f v1);
Vector3f vec3f_addf(Vector3f v, float f);
Vector3f vec3f_sub(Vector3f v0, Vector3f v1);
Vector3f vec3f_subf(Vector3f v, float f);
Vector3f vec3f_mul(Vector3f v0, Vector3f v1);
Vector3f vec3f_mulf(Vector3f v, float f);
Vector3f vec3f_div(Vector3f v0, Vector3f v1);
Vector3f vec3f_divf(Vector3f v, float f);
Vector3f vec3f_norm(Vector3f v);
Vector3f vec3f_neg(Vector3f v);
Vector3f vec3f_cross(Vector3f v0, Vector3f v1);
float vec3f_len(Vector3f v);
float vec3f_len_sqr(Vector3f v);
float vec3f_dot(Vector3f v0, Vector3f v1);

void vec4f_print(Vector4f v);
Vector4f vec4f_set(float x, float y, float z, float w);
Vector4f vec4f_add(Vector4f v0, Vector4f v1);
Vector4f vec4f_addf(Vector4f v, float f);
Vector4f vec4f_sub(Vector4f v0, Vector4f v1);
Vector4f vec4f_subf(Vector4f v, float f);
Vector4f vec4f_mul(Vector4f v0, Vector4f v1);
Vector4f vec4f_mulf(Vector4f v, float f);
Vector4f vec4f_mulm4(Vector4f v, Matrix4f m);
Vector4f vec4f_div(Vector4f v0, Vector4f v1);
Vector4f vec4f_divf(Vector4f v, float f);
Vector4f vec4f_norm(Vector4f v);
Vector4f vec4f_neg(Vector4f v);
Vector4f vec4f_cross(Vector4f v0, Vector4f v1);
float vec4f_len(Vector4f v);
float vec4f_len_sqr(Vector4f v);
float vec4f_dot(Vector4f v0, Vector4f v1);

#endif
