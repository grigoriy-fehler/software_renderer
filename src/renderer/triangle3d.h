#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include "line3d.h"
#include "vertex3d.h"
#include "framebuffer.h"
#include "texture.h"
#include "color_rgba.h"
#include "../math/mathlib.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define triangle3d(p1, p2, p3, texture) (triangle3d_t) { \
	(vertex3d_t) (p1), \
	(vertex3d_t) (p2), \
	(vertex3d_t) (p3), \
	(texture_t *) (texture), \
}

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct triangle3d_t {
	vertex3d_t p1, p2, p3;
	texture_t* texture;
} triangle3d_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void triangle3d_stroke(framebuffer_t* fb, triangle3d_t* triangle)
{
	line3d_t l1 = line3d(triangle->p1, triangle->p2);
	line3d_t l2 = line3d(triangle->p2, triangle->p3);
	line3d_t l3 = line3d(triangle->p3, triangle->p1);

	line3d_stroke(fb, &l1);
	line3d_stroke(fb, &l2);
	line3d_stroke(fb, &l3);
} // triangle3d_stroke

static inline void triangle3d_fill(framebuffer_t* fb,
	triangle3d_t* triangle)
{
	texture_t* tex = triangle->texture;
	vertex3d_t* v1 = &triangle->p1;
	vertex3d_t* v2 = &triangle->p2;
	vertex3d_t* v3 = &triangle->p3;

	if (v1->position.y > v2->position.y)
		vertex3d_swap(v1, v2);
	if (v1->position.y > v3->position.y)
		vertex3d_swap(v1, v3);
	if (v2->position.y > v3->position.y)
		vertex3d_swap(v2, v3);

	vector2d_multiply_float(&v1->texcoord, &v1->texcoord, v1->position.z);
	vector2d_multiply_float(&v2->texcoord, &v2->texcoord, v2->position.z);
	vector2d_multiply_float(&v3->texcoord, &v3->texcoord, v3->position.z);

	vector4d_multiply_float(&v1->normal, &v1->normal, v1->position.z);
	vector4d_multiply_float(&v2->normal, &v2->normal, v2->position.z);
	vector4d_multiply_float(&v3->normal, &v3->normal, v3->position.z);

	vector3d_multiply_float(&v1->color.rgb, &v1->color.rgb, v1->position.z);
	vector3d_multiply_float(&v2->color.rgb, &v2->color.rgb, v2->position.z);
	vector3d_multiply_float(&v3->color.rgb, &v3->color.rgb, v3->position.z);

	vertex3d_t v, vl, vr;
	i32 p1y = floor(v1->position.y);
	i32 p2y = floor(v2->position.y);
	i32 p3y = floor(v3->position.y);
	for (f32 y = p1y; y < p3y; y++) {
		if (y < p2y) {
			f32 dl = (f32) (y - p1y) / (p2y - p1y);
			vertex3d_lerp(&vl, v1, v2, dl);
		} else {
			f32 dl = (f32) (y - p2y) / (p3y - p2y);
			vertex3d_lerp(&vl, v2, v3, dl);
		}

		f32 dr = (f32) (y - p1y) / (p3y - p1y);
		vertex3d_lerp(&vr, v1, v3, dr);

		if (vl.position.x > vr.position.x)
			vertex3d_swap(&vl, &vr);

		i32 xl = floor(vl.position.x);
		i32 xr = floor(vr.position.x);
		for (i32 x = xl; x < xr; x++) {
			f32 x_norm = (f32) (x - xl) / (xr - xl);
			f32 z = lerp(vl.position.z, vr.position.z, x_norm);
			f32 z_inv = 1.0f / z;
			if (get_depth(fb, x, y) < z_inv) continue;

			vertex3d_lerp(&v, &vl, &vr, x_norm);

			vector2d_multiply_float(
				&v.texcoord,
				&v.texcoord,
				z_inv
			);
			vector4d_multiply_float(&v.normal, &v.normal, z_inv);
			vector3d_multiply_float(
				&v.color.rgb,
				&v.color.rgb,
				z_inv
			);

			i32 tu = v.texcoord.u * tex->width;
			i32 tv = v.texcoord.v * tex->height;

			i32 index = (tv * tex->width + tu);
			index = clamp(index, 0, tex->width * tex->height - 1);
			color_rgba_t c = tex->data[index];
			vector3d_multiply(&c.rgb, &c.rgb, &v.color.rgb);
			if (c.a < 0.1f) continue;
			set_depth(fb, x, y, z_inv);
			set_pixel(fb, x, y, &c);
		}
	}
} // triangle3d_fill

static inline i32 triangle3d_clip(vertex3d_t* out, vertex3d_t* in, plane3d_t* p,
	i32 in_count)
{
	i32 vertex_count = 0;
	vertex3d_t* in_vertex = in;
	vertex3d_t* out_vertex = out;

	f32 current_dot = vector3d_dot_product(&in[0].position.xyz, &p->normal);
	i32 current_inside = (current_dot >= p->distance);

	for (i32 i = 0; i < in_count; i++) {
		i32 next_vert = (i + 1) % in_count;

		if (current_inside) {
			*out_vertex = *in_vertex;
			out_vertex++;
			vertex_count++;
		}

		f32 next_dot = vector3d_dot_product(
			&in[next_vert].position.xyz, &p->normal
		);
		i32 next_inside = (next_dot >= p->distance);

		if (current_inside != next_inside) {
			f32 t = (p->distance - current_dot) / 
				(next_dot - current_dot);
			vertex3d_lerp(out_vertex, in_vertex, &in[next_vert], t);
			out_vertex++;
			vertex_count++;
		}
		current_dot = next_dot;
		current_inside = next_inside;
		in_vertex++;
	}

	return vertex_count;
} // triangle3d_clip

#endif // TRIANGLE3D_H
