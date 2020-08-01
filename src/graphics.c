#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "graphics.h"
#include "math/maths.h"
#include "utils.h"

Vector3f world_to_screen(Vector3f v, uint16_t width, uint16_t height) {
	Vector3f ret;
	
	ret.x = (v.x + 1.0f) * width / 2;
	ret.y = (v.y + 1.0f) * height / 2;
	ret.z = v.z;
	
	return ret;
}

Context* ctx_create(uint16_t width, uint16_t height) {
	Context* ctx;

	Color c = { 0, 0, 0, 255 };
	float d = INFINITY;

	ctx = (Context *) malloc(sizeof(Context));
	ctx->w = width;
	ctx->h = height;
	ctx->color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * ctx->w * ctx->h);
	ctx->depth_buffer = (float *) malloc(sizeof(float) * ctx->w * ctx->h);

	ctx_clear_color(ctx, c);
	ctx_clear_depth(ctx, d);

	return ctx;
}

void ctx_free(Context* ctx) {
	free(ctx->color_buffer);
	free(ctx->depth_buffer);
	free(ctx);
}

void ctx_clear_color(Context* ctx, Color c) {
	for (int i = 0; i < ctx->w * ctx->h; i++)
		ctx->color_buffer[i] = color_to_uint(c);
}

void ctx_clear_depth(Context* ctx, float depth) {
	for (int i = 0; i < ctx->w * ctx->h; i++)
		ctx->depth_buffer[i] = depth;
}

void ctx_put_pixel(Context* ctx, uint16_t x, uint16_t y, Color c) {
	if (x > ctx->w) return;
	if (y > ctx->h) return;

	ctx->color_buffer[ctx->w * y + x] = color_to_uint(c);
}

void ctx_triangle(Context* ctx, Vector3f v[], Vector2f t[], Color c) {
	if (v[0].y > v[1].y) {
		swapf(&v[0].x, &v[1].x);
		swapf(&v[0].y, &v[1].y);
	}
	if (v[0].y > v[2].y) {
		swapf(&v[0].x, &v[2].x);
		swapf(&v[0].y, &v[2].y);
	}
	if (v[1].y > v[2].y) {
		swapf(&v[1].x, &v[2].x);
		swapf(&v[1].y, &v[2].y);
	}

	Vector2 v0 = { (int) v[0].x, (int) v[0].y };
	Vector2 v1 = { (int) v[1].x, (int) v[1].y };
	Vector2 v2 = { (int) v[2].x, (int) v[2].y };

	for (int y = v0.y; y < v2.y; y++) {
		int x0;
		if (y < v1.y) {
			x0 = vec2_interpolate(v0, v1, y);
		} else {
			x0 = vec2_interpolate(v1, v2, y);
		}

		int x1 = vec2_interpolate(v0, v2, y);

		if (x0 > x1) swap(&x0, &x1);
		for (int x = x0; x < x1; x++) {
			ctx_put_pixel(ctx, x, y, c);
		}
	}
}

Camera* cam_create(uint8_t fov, float near, float far, Vector3f pos) {
	Camera* c = (Camera *) malloc(sizeof(Camera));
	c->fov = fov;
	c->near = near;
	c->far = far;
	c->pos = pos;
	return c;
}

void cam_free(Camera* cam) {
	free(cam);
}

Scene* scn_create(Model* models, uint32_t mcount, Camera* cam) {
	Scene* s;

	s = (Scene *) malloc(sizeof(Scene));
	s->models = models;
	s->model_count = mcount;
	s->camera = cam;

	return s;
}

void scn_free(Scene* scn) {
	free(scn->models);
	free(scn->camera);
	free(scn);
}

void scn_render(Context* ctx, Scene* scn) {
	ctx_clear_color(ctx, BLACK);
	ctx_clear_depth(ctx, INFINITY);

	for (uint32_t i = 0; i < scn->model_count; i++) {
		for (uint32_t j = 0; j < scn->models[i].mesh_count; j++) {
			for (uint32_t k = 0; k < scn->models[i].meshes[j].index_count; k += 3) {
				Vector3f screen_coords[3];
				Vector2f tex_coords[3];
				Vector3f world_coords[3];
				Vector3f normals[3];
				Vector3f light_dir = { 0, 0, -1 };
				
				for (int l = 0; l < 3; l++) {
					int vi = scn->models[i].meshes[j].indices[k+l].v * 3;
					Vector4f v = {
						scn->models[i].meshes[j].vertices[vi],
						scn->models[i].meshes[j].vertices[vi + 1],
						scn->models[i].meshes[j].vertices[vi + 2],
						1.0f
					};
					
					int ti = scn->models[i].meshes[j].indices[k+l].t * 2;
					Vector2f t = {
						scn->models[i].meshes[j].texcoords[ti],
						scn->models[i].meshes[j].texcoords[ti + 1]
					};
					tex_coords[l] = t;
					
					int ni = scn->models[i].meshes[j].indices[k+l].n * 3;
					Vector3f n = {
						scn->models[i].meshes[j].normals[ni],
						scn->models[i].meshes[j].normals[ni + 1],
						scn->models[i].meshes[j].normals[ni + 2]
					};
					normals[l] = n;
					
					v = vec4f_mulm4(v, scn->models[i].matrix);
					v = vec4f_mulm4(v, mat4f_proj(scn->camera->near, scn->camera->far, (float) ctx->w / ctx->h * 2, 2));
					v = vec4f_divf(v, v.w);
					
					world_coords[l] = vec3f_set(v.x, v.y, v.z);
					screen_coords[l] = world_to_screen(world_coords[l], ctx->w, ctx->h);
				}
				
				Vector3f n = vec3f_cross(vec3f_sub(world_coords[1], world_coords[0]), vec3f_sub(world_coords[2], world_coords[0]));
				float intensity = vec3f_dot(light_dir, vec3f_norm(n));
				
				if (intensity >= 0.0f) {
					Color c = { intensity * 255, intensity * 255, intensity * 255, 255 };
					ctx_triangle(ctx, screen_coords, tex_coords, c);
				}
			}
		}
	}
}
