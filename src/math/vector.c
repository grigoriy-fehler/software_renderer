#include <stdio.h>
#include <math.h>

#include "maths.h"
#include "vector.h"

void vec2_print(Vector2 v) {
    printf("[x: %d, y: %d]\n", v.x, v.y);
}

Vector2 vec2_set(int x, int y) {
	Vector2 ret = { x, y };
	return ret;
}

Vector2 vec2_add(Vector2 v0, Vector2 v1) {
	Vector2 ret = { v0.x + v1.x, v0.y + v1.y };
	return ret;
}

Vector2 vec2_addi(Vector2 v, int i) {
	Vector2 ret = { v.x + i, v.y + i };
	return ret;
}

Vector2 vec2_sub(Vector2 v0, Vector2 v1) {
	Vector2 ret = { v0.x - v1.x, v0.y - v1.y };
	return ret;
}

Vector2 vec2_subi(Vector2 v, int i) {
	Vector2 ret = { v.x - i, v.y - i };
	return ret;
}

Vector2 vec2_mul(Vector2 v0, Vector2 v1) {
	Vector2 ret = { v0.x * v1.x, v0.y * v1.y };
	return ret;
}

Vector2 vec2_muli(Vector2 v, int i) {
	Vector2 ret = { v.x * i, v.y * i };
	return ret;
}

Vector2 vec2_div(Vector2 v0, Vector2 v1) {
	Vector2 ret = { v0.x / v1.x, v0.y / v1.y };
	return ret;
}

Vector2 vec2_divi(Vector2 v, int i) {
	Vector2 ret = { v.x / i, v.y / i };
	return ret;
}

Vector2 vec2_neg(Vector2 v) {
	Vector2 ret = { -v.x, -v.y };
	return ret;
}

float vec2_dist(Vector2 v0, Vector2 v1) {
	return sqrtf((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y));
}

float vec2_len(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

int vec2_len_sqr(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

int vec2_dot(Vector2 v0, Vector2 v1) {
	return v0.x * v1.x + v0.y * v1.y;
}

int vec2_angl(Vector2 v0, Vector2 v1) {
	int ret = (int) atan2f(v1.y - v0.y, v1.x - v0.x) * (180.0f/PI);
	if (ret < 0) ret += 360;
	return ret;
}

int vec2_interpolate(Vector2 v0, Vector2 v1, int y) {
	return v0.x + (v1.x - v0.x) * (y - v0.y) / (v1.y - v0.y);
}

void vec2f_print(Vector2f v) {
    printf("[x: %f, y: %f]\n", v.x, v.y);
}

Vector2f vec2f_set(float x, float y) {
	Vector2f ret = { x, y };
	return ret;
}

Vector2f vec2f_add(Vector2f v0, Vector2f v1) {
	Vector2f ret = { v0.x + v1.x, v0.y + v1.y };
	return ret;
}

Vector2f vec2f_addf(Vector2f v, float f) {
	Vector2f ret = { v.x + f, v.y + f };
	return ret;
}

Vector2f vec2f_sub(Vector2f v0, Vector2f v1) {
	Vector2f ret = { v0.x - v1.x, v0.y - v1.y };
	return ret;
}

Vector2f vec2f_subf(Vector2f v, float f) {
	Vector2f ret = { v.x - f, v.y - f };
	return ret;
}

Vector2f vec2f_mul(Vector2f v0, Vector2f v1) {
	Vector2f ret = { v0.x * v1.x, v0.y * v1.y };
	return ret;
}

Vector2f vec2f_mulf(Vector2f v, float f) {
	Vector2f ret = { v.x * f, v.y * f };
	return ret;
}

Vector2f vec2f_div(Vector2f v0, Vector2f v1) {
	Vector2f ret = { v0.x / v1.x, v0.y / v1.y };
	return ret;
}

Vector2f vec2f_divf(Vector2f v, float f) {
	Vector2f ret = { v.x / f, v.y / f };
	return ret;
}

Vector2f vec2f_neg(Vector2f v) {
	Vector2f ret = { -v.x, -v.y };
	return ret;
}

float vec2f_len(Vector2f v) {
	return sqrtf((v.x * v.x) + (v.y * v.y));
}

float vec2f_len_sqr(Vector2f v) {
	return (v.x * v.x) + (v.y * v.y);
}

float vec2f_dot(Vector2f v0, Vector2f v1) {
	return v0.x * v1.x + v0.y * v1.y;
}

float vec2f_dist(Vector2f v0, Vector2f v1) {
	return sqrtf((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y));
}

float vec2f_angl(Vector2f v0, Vector2f v1) {
	float ret = atan2f(v1.y - v0.y, v1.x - v0.x) * (180.0f/PI);
	if (ret < 0) ret += 360;
	return ret;
}

float vec2f_interpolate(Vector2f v0, Vector2f v1, float y) {
	return v0.x + (v1.x - v0.x) * (y - v0.y) / (v1.y - v0.y);
}

void vec3f_print(Vector3f v) {
	printf("[x: %f, y: %f, z: %f]\n", v.x, v.y, v.z);
}

Vector3f vec3f_set(float x, float y, float z) {
	Vector3f ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

Vector3f vec3f_add(Vector3f v0, Vector3f v1) {
	Vector3f ret = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z };
	return ret;
}

Vector3f vec3f_addf(Vector3f v, float f) {
	Vector3f ret = { v.x + f, v.y + f, v.z + f };
	return ret;
}

Vector3f vec3f_sub(Vector3f v0, Vector3f v1) {
	Vector3f ret = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z };
	return ret;
}

Vector3f vec3f_mul(Vector3f v0, Vector3f v1) {
	Vector3f ret = { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z };
	return ret;
}

Vector3f vec3f_mulf(Vector3f v, float f) {
	Vector3f ret = { v.x * f, v.y * f, v.z * f };
	return ret;
}

Vector3f vec3f_div(Vector3f v0, Vector3f v1) {
	Vector3f ret = { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z };
	return ret;
}

Vector3f vec3f_divf(Vector3f v, float f) {
	Vector3f ret = { v.x / f, v.y / f, v.z / f };
	return ret;
}

Vector3f vec3f_norm(Vector3f v) {
	Vector3f ret;
	
	float length = vec3f_len(v);
	
	ret.x = v.x / length;
	ret.y = v.y / length;
	ret.z = v.z / length;
	
	return ret;
}

Vector3f vec3f_neg(Vector3f v) {
	Vector3f ret = { -v.x, -v.y, -v.z };
	return ret;
}

Vector3f vec3f_cross(Vector3f v0, Vector3f v1) {
	Vector3f ret = {
		v0.y * v1.z - v0.z * v1.y,
		v0.z * v1.x - v0.x * v1.z,
		v0.x * v1.y - v0.y * v1.x
	};
	return ret;
}

float vec3f_len(Vector3f v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec3f_len_sqr(Vector3f v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3f_dot(Vector3f v0, Vector3f v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

void vec4f_print(Vector4f v) {
    printf("[x: %.2f, y: %.2f, z: %.2f, w: %.2f]\n", v.x, v.y, v.z, v.w);
}

Vector4f vec4f_set(float x, float y, float z, float w) {
	Vector4f ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	ret.w = w;
	return ret;
}

Vector4f vec4f_add(Vector4f v0, Vector4f v1) {
	Vector4f ret = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w };
	return ret;
}

Vector4f vec4f_addf(Vector4f v, float f) {
	Vector4f ret = { v.x + f, v.y + f, v.z + f, v.w + f };
	return ret;
}

Vector4f vec4f_sub(Vector4f v0, Vector4f v1) {
	Vector4f ret = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w };
	return ret;
}

Vector4f vec4f_mul(Vector4f v0, Vector4f v1) {
	Vector4f ret = { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w };
	return ret;
}

Vector4f vec4f_mulf(Vector4f v, float f) {
	Vector4f ret = { v.x * f, v.y * f, v.z * f, v.w * f };
	return ret;
}

Vector4f vec4f_mulm4(Vector4f v, Matrix4f m) {
    Vector4f ret;
    ret.x = m.xx * v.x + m.xy * v.y + m.xz * v.z + m.xw * v.w;
    ret.y = m.yx * v.x + m.yy * v.y + m.yz * v.z + m.yw * v.w;
    ret.z = m.zx * v.x + m.zy * v.y + m.zz * v.z + m.zw * v.w;
    ret.w = m.wx * v.x + m.wy * v.y + m.wz * v.z + m.ww * v.w;
    return ret;
}

Vector4f vec4f_div(Vector4f v0, Vector4f v1) {
	Vector4f ret = { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z, v0.w / v1.w };
	return ret;
}

Vector4f vec4f_divf(Vector4f v, float f) {
	Vector4f ret = { v.x / f, v.y / f, v.z / f, v.w / f };
	return ret;
}

Vector4f vec4f_trans(Matrix4f m, Vector4f v) {
    Vector4f ret;
    ret.x = m.xx * v.x + m.xy * v.y + m.xz * v.z + m.xw * v.w;
    ret.y = m.yx * v.x + m.yy * v.y + m.yz * v.z + m.yw * v.w;
    ret.z = m.zx * v.x + m.zy * v.y + m.zz * v.z + m.zw * v.w;
    ret.w = m.wx * v.x + m.wy * v.y + m.wz * v.z + m.ww * v.w;
    return ret;
}

float vec4f_len(Vector4f v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

Vector4f vec4f_norm(Vector4f v) {
	Vector4f ret;
	
	float length = vec4f_len(v);
	
	ret.x = v.x / length;
	ret.y = v.y / length;
	ret.z = v.z / length;
	ret.w = v.w / length;
	
	return ret;
}

Vector4f vec4f_neg(Vector4f v) {
	Vector4f ret = { -v.x, -v.y, -v.z, -v.w };
	return ret;
}

Vector4f vec4f_cross(Vector4f v0, Vector4f v1) {
	Vector4f ret = {
		v0.y * v1.z - v0.z * v1.y,
		v0.z * v1.x - v0.x * v1.z,
		v0.x * v1.y - v0.y * v1.x,
		1
	};
	return ret;
}

float vec4f_dot(Vector4f v0, Vector4f v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}

