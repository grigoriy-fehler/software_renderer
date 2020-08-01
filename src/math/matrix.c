#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "maths.h"
#include "matrix.h"

void mat4f_print(Matrix4f m) {
	printf("|xx: %.2f, xy: %.2f, xz: %.2f, xw: %.2f|\n", m.xx, m.xy, m.xz, m.xw);
	printf("|yx: %.2f, yy: %.2f, yz: %.2f, yw: %.2f|\n", m.yx, m.yy, m.yz, m.yw);
	printf("|zx: %.2f, zy: %.2f, zz: %.2f, zw: %.2f|\n", m.zx, m.zy, m.zz, m.zw);
	printf("|wx: %.2f, wy: %.2f, wz: %.2f, ww: %.2f|\n", m.wx, m.wy, m.wz, m.ww);
}

Matrix4f mat4f_identity() {
	Matrix4f m = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return m;
}

Matrix4f mat4f_add(Matrix4f m0, Matrix4f m1) {
	Matrix4f m;
	m.xx = m0.xx * m1.xx;
	m.xy = m0.xy * m1.xy;
	m.xz = m0.xz * m1.xz;
	m.xw = m0.xw * m1.xw;

	m.yx = m0.yx * m1.xx;
	m.yy = m0.yx * m1.xy;
	m.yz = m0.yx * m1.xz;
	m.yw = m0.yx * m1.xw;

	m.zx = m0.zx * m1.xx;
	m.zy = m0.zx * m1.xy;
	m.zz = m0.zx * m1.xz;
	m.zw = m0.zx * m1.xw;
	
	m.wx = m0.wx * m1.xx;
	m.wy = m0.wx * m1.xy;
	m.wz = m0.wx * m1.xz;
	m.ww = m0.wx * m1.xw;
	return m;
}

Matrix4f mat4f_mul(Matrix4f m0, Matrix4f m1) {
	Matrix4f m;
	m.xx = m0.xx * m1.xx + m0.xy * m1.yx + m0.xz * m1.zx + m0.xw * m1.wx;
	m.xy = m0.xx * m1.xy + m0.xy * m1.yy + m0.xz * m1.zy + m0.xw * m1.wy;
	m.xz = m0.xx * m1.xz + m0.xy * m1.yz + m0.xz * m1.zz + m0.xw * m1.wz;
	m.xw = m0.xx * m1.xw + m0.xy * m1.yw + m0.xz * m1.zw + m0.xw * m1.ww;
	
	m.yx = m0.yx * m1.xx + m0.yy * m1.yx + m0.yz * m1.zx + m0.yw * m1.wx;
	m.yy = m0.yx * m1.xy + m0.yy * m1.yy + m0.yz * m1.zy + m0.yw * m1.wy;
	m.yz = m0.yx * m1.xz + m0.yy * m1.yz + m0.yz * m1.zz + m0.yw * m1.wz;
	m.yw = m0.yx * m1.xw + m0.yy * m1.yw + m0.yz * m1.zw + m0.yw * m1.ww;
	
	m.zx = m0.zx * m1.xx + m0.zy * m1.yx + m0.zz * m1.zx + m0.zw * m1.wx;
	m.zy = m0.zx * m1.xy + m0.zy * m1.yy + m0.zz * m1.zy + m0.zw * m1.wy;
	m.zz = m0.zx * m1.xz + m0.zy * m1.yz + m0.zz * m1.zz + m0.zw * m1.wz;
	m.zw = m0.zx * m1.xw + m0.zy * m1.yw + m0.zz * m1.zw + m0.zw * m1.ww;
	
	m.wx = m0.wx * m1.xx + m0.wy * m1.yx + m0.wz * m1.zx + m0.ww * m1.wx;
	m.wy = m0.wx * m1.xy + m0.wy * m1.yy + m0.wz * m1.zy + m0.ww * m1.wy;
	m.wz = m0.wx * m1.xz + m0.wy * m1.yz + m0.wz * m1.zz + m0.ww * m1.wz;
	m.ww = m0.wx * m1.xw + m0.wy * m1.yw + m0.wz * m1.zw + m0.ww * m1.ww;
	return m;
}

Matrix4f mat4f_trans(Matrix4f m, float x, float y, float z) {
	m.xw = x;
	m.yw = y;
	m.zw = z;
	return m;
}

Matrix4f mat4f_scale(float x, float y, float z) {
	Matrix4f m = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
	return m;
}

Matrix4f mat4f_rot_x(float r) {
	Matrix4f m = {
		1,       0,      0, 0,
		0,  cos(r), sin(r), 0,
		0, -sin(r), cos(r), 0,
		0,       0,      0, 1
	};
	return m;
}

Matrix4f mat4f_rot_y(float r) {
	Matrix4f m = {
		cos(r), 0, -sin(r), 0,
		     0, 1,       0, 0,
		sin(r), 0,  cos(r), 0,
		     0, 0,       0, 1
	};
	return m;
}

Matrix4f mat4f_rot_z(float r) {
	Matrix4f m = {
		 cos(r), sin(r), 0, 0,
		-sin(r), cos(r), 0, 0,
		      0,      0, 1, 0,
		      0,      0, 0, 1
	};
	return m;
}

Matrix4f mat4f_proj(float near, float far, float width, float height) {
	Matrix4f m = {
		2 * near / width, 0, 0, 0,
		0, 2 * near / height, 0, 0,
		0, 0, far / (far - near), 1,
		0, 0, -far * near / (far - near), 0
	};
	return m;
}
