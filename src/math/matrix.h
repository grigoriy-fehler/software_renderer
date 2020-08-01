#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix4f {
    float xx, xy, xz, xw;
    float yx, yy, yz, yw;
    float zx, zy, zz, zw;
    float wx, wy, wz, ww;
} Matrix4f;

void mat4f_print(Matrix4f m);
Matrix4f mat4f_identity();
Matrix4f mat4f_add(Matrix4f m0, Matrix4f m1);
Matrix4f mat4f_sub();
Matrix4f mat4f_mul(Matrix4f m0, Matrix4f m1);
Matrix4f mat4f_div();
Matrix4f mat4f_norm();
Matrix4f mat4f_trans(Matrix4f m, float x, float y, float z);
Matrix4f mat4f_scale(float x, float y, float z);
Matrix4f mat4f_inv(Matrix4f m);
Matrix4f mat4f_rot_x(float r);
Matrix4f mat4f_rot_y(float r);
Matrix4f mat4f_rot_z(float r);
Matrix4f mat4f_proj(float near, float far, float width, float height);

#endif
