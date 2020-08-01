#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

#include "math/vector.h"
#include "math/matrix.h"
#include "color.h"
#include "model.h"

#include "third_party/fast_obj.h"

void set_col_arr();

typedef struct Context {
    uint16_t w, h;
    uint32_t* color_buffer;
    float* depth_buffer;
} Context;

typedef struct {
    uint8_t fov;
    float near;
    float far;
    Vector3f pos;
} Camera;

typedef struct {
    Model* models;
    uint32_t model_count;
    Camera* camera;
} Scene;

Context* ctx_create(uint16_t width, uint16_t height);
void ctx_free(Context* ctx);
void ctx_clear_color(Context* ctx, Color c);
void ctx_clear_depth(Context* ctx, float depth);
void ctx_put_pixel(Context* ctx, uint16_t x, uint16_t y, Color c);
void ctx_triangle(Context* ctx, Vector3f v[], Vector2f t[], Color c);

Camera* cam_create(uint8_t fov, float near, float far, Vector3f pos);
void cam_free(Camera* cam);

Scene* scn_create(Model* models, uint32_t mcount, Camera* cam);
void scn_free(Scene* scn);
void scn_render(Context* ctx, Scene* scn);

#endif
