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

static inline void triangle_stroke(framebuffer_t* fb, triangle3d_t* triangle) {
	line3d_t l1 = line3d(triangle->p1, triangle->p2);
	line3d_t l2 = line3d(triangle->p2, triangle->p3);
	line3d_t l3 = line3d(triangle->p3, triangle->p1);

	line_draw(fb, &l1);
	line_draw(fb, &l2);
	line_draw(fb, &l3);
} // triangle_stroke

static inline void triangle_fill(framebuffer_t* fb, triangle3d_t* triangle) {
	texture_t* tex = triangle->texture;
	i32 p1x = triangle->p1.position.x;
	i32 p1y = triangle->p1.position.y;
	f32 p1z = triangle->p1.position.z;
	i32 p2x = triangle->p2.position.x;
	i32 p2y = triangle->p2.position.y;
	f32 p2z = triangle->p2.position.z;
	i32 p3x = triangle->p3.position.x;
	i32 p3y = triangle->p3.position.y;
	f32 p3z = triangle->p3.position.z;
	f32 tc1u = triangle->p1.texcoord.u;
	f32 tc1v = triangle->p1.texcoord.v;
	f32 tc2u = triangle->p2.texcoord.u;
	f32 tc2v = triangle->p2.texcoord.v;
	f32 tc3u = triangle->p3.texcoord.u;
	f32 tc3v = triangle->p3.texcoord.v;
	color_rgba_t color1 = triangle->p1.color;
	color_rgba_t color2 = triangle->p2.color;
	color_rgba_t color3 = triangle->p3.color;

	if (p1y > p2y) {
		swapi(&p1x, &p2x);
		swapi(&p1y, &p2y);
		swapf(&p1z, &p2z);
		swapf(&tc1u, &tc2u);
		swapf(&tc1v, &tc2v);
		vector4d_swap(&color1.rgba, &color2.rgba);
	}
	if (p1y > p3y) {
		swapi(&p1x, &p3x);
		swapi(&p1y, &p3y);
		swapf(&p1z, &p3z);
		swapf(&tc1u, &tc3u);
		swapf(&tc1v, &tc3v);
		vector4d_swap(&color1.rgba, &color3.rgba);
	}
	if (p2y > p3y) {
		swapi(&p2x, &p3x);
		swapi(&p2y, &p3y);
		swapf(&p2z, &p3z);
		swapf(&tc2u, &tc3u);
		swapf(&tc2v, &tc3v);
		vector4d_swap(&color2.rgba, &color3.rgba);
	}

	for (f32 y = p1y; y < p3y; y++) {
		i32 xl;
		f32 zl;
		f32 ul;
		f32 vl;
		color_rgba_t cl = color_rgba(0.0f, 0.0f, 0.0f, 1.0f);
		if (y < p2y) {
			f32 y_norm = (f32) (y - p1y) / (p2y - p1y);
			xl = lerp(p1x, p2x, y_norm);
			zl = lerp(p1z, p2z, y_norm);
			ul = lerp(tc1u * p1z, tc2u * p2z, y_norm);
			vl = lerp(tc1v * p1z, tc2v * p2z, y_norm);
			vector3d_lerp(&cl.rgb, &color1.rgb, &color2.rgb, y_norm);
		} else {
			f32 y_norm = (f32) (y - p2y) / (p3y - p2y);
			xl = lerp(p2x, p3x, y_norm);
			zl = lerp(p2z, p3z, y_norm);
			ul = lerp(tc2u * p2z, tc3u * p3z, y_norm);
			vl = lerp(tc2v * p2z, tc3v * p3z, y_norm);
			vector3d_lerp(&cl.rgb, &color2.rgb, &color3.rgb, y_norm);
		}

		f32 y_norm = (f32) (y - p1y) / (p3y - p1y);
		i32 xr = lerp(p1x, p3x, y_norm);
		f32 zr = lerp(p1z, p3z, y_norm);
		f32 ur = lerp(tc1u * p1z, tc3u * p3z, y_norm);
		f32 vr = lerp(tc1v * p1z, tc3v * p3z, y_norm);
		color_rgba_t cr = color_rgba(0.0f, 0.0f, 0.0f, 1.0f);
		vector3d_lerp(&cr.rgb, &color1.rgb, &color3.rgb, y_norm);

		if (xl > xr) {
			swapi(&xl, &xr);
			swapf(&zl, &zr);
			swapf(&ul, &ur);
			swapf(&vl, &vr);
			vector4d_swap(&cl.rgba, &cr.rgba);
		}

		for (i32 x = xl; x < xr; x++) {
			f32 x_norm = (f32) (x - xl) / (xr - xl);
			f32 z = lerp(zl, zr, x_norm);
			f32 z_inv = 1.0f / z;
			if (get_depth(fb, x, y) < z_inv) continue;

			i32 u = lerp(ul * z_inv, ur * z_inv, x_norm) * tex->width;
			i32 v = lerp(vl * z_inv, vr * z_inv, x_norm) * tex->height;
			color_rgba_t color = color_rgba(0.0f, 0.0f, 0.0f, 1.0f);
			vector3d_lerp(&color.rgb, &cl.rgb, &cr.rgb, x_norm);

			i32 index = (v * tex->width + u);
			index = clamp(index, 0, tex->width * tex->height - 1);
			color_rgba_t c = tex->data[index];
			vector3d_multiply(&c.rgb, &c.rgb, &color.rgb);
			if (c.a < 0.1f) continue;
			set_depth(fb, x, y, z_inv);
			set_pixel(fb, x, y, &c);
		}
	}
} // triangle_fill

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
			f32 scale = (p->distance - current_dot) / 
				(next_dot - current_dot);
			for (i32 j = 0; j < 3; j++) {
				vector4d_lerp(
					&out_vertex->position,
					&in_vertex->position,
					&in[next_vert].position,
					scale
				);
			}
			for (i32 j = 0; j < 2; j++) {
				vector2d_lerp(
					&out_vertex->texcoord,
					&in_vertex->texcoord,
					&in[next_vert].texcoord,
					scale
				);
			}
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
