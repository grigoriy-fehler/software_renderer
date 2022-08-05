#ifndef RENDERER_H
#define RENDERER_H

#include "maths.h"

// R E N D E R E R /////////////////////////////////////////////////////////////

#define color_rgba(r, g, b, a) (color_rgba_t) {{ \
	(f32) (r), \
	(f32) (g), \
	(f32) (b), \
	(f32) (a) \
}}
#define texture(width, height, data) (texture_t) { \
	(i32) (width), \
	(i32) (height), \
	(color_rgba_t *) (data) \
}
#define material(texture_index) (material_t) { (u32) (texture_index) }
#define vertex3d(position, texcoord) (vertex3d_t) { \
	(point4d_t) (position), \
	(point2d_t) (texcoord) \
}
#define index3d(position, texcoord) (index3d_t) { \
	(i32) (position), \
	(i32) (texcoord) \
}
#define line3d(start, end, color) (line3d_t) { \
	(vertex3d_t) (start), \
	(vertex3d_t) (end), \
	(color_rgba_t *) (color) \
}
#define plane3d(distance, normal) (plane3d_t) { \
	(f32) (distance), \
	(vector3d_t) (normal) \
}
#define triangle3d(p1, p2, p3, texture, color) (triangle3d_t) { \
	(vertex3d_t) (p1), \
	(vertex3d_t) (p2), \
	(vertex3d_t) (p3), \
	(texture_t *) (texture), \
	(color_rgba_t *) (color) \
}
#define face3d(color, index_count, indices) (face3d_t) { \
	(color_rgba_t *) (color), \
	(u32) (index_count), \
	(index3d_t *) (indices) \
}

typedef union color_rgba_t {
	struct { f32 r, g, b, a; };
	vector4d_t rgba;
	vector3d_t rgb;
	f32 e[4];
} color_rgba_t;

typedef struct texture_t {
	i32 width;
	i32 height;
	color_rgba_t* data;
} texture_t;

typedef struct material_t {
	u32 texture_index;
} material_t;

typedef struct vertex3d_t {
	point4d_t position;
	point2d_t texcoord;
} vertex3d_t;

typedef struct index3d_t {
	i32 position;
	i32 texcoord;
} index3d_t;

typedef struct line3d_t {
	vertex3d_t start, end;
	color_rgba_t* color;
} line3d_t;

typedef struct plane3d_t {
	f32 distance;
	vector3d_t normal;
} plane3d_t;

typedef struct triangle3d_t {
	vertex3d_t p1, p2, p3;
	texture_t* texture;
	color_rgba_t* color;
} triangle3d_t;

typedef struct face3d_t {
	color_rgba_t* color;
	u32 index_count;
	index3d_t* indices;
} face3d_t;

typedef struct render_entity_t {
	u32 vertex_count;
	point4d_t* vertices;
	u32 texcoord_count;
	point2d_t* texcoords;
	u32 face_count;
	face3d_t* faces;
	u32 material_index;
	transform4d_t transform;
} render_entity_t;

typedef enum image_format_t {
	IMAGE_FORMAT_RGBA = 0,
	IMAGE_FORMAT_ARGB,
	IMAGE_FORMAT_ABGR
} image_format_t;

typedef struct framebuffer_t {
	image_format_t image_format;
	i32 width;
	i32 height;
	f32* depth;
	u32* color;
} framebuffer_t;

typedef struct camera_t {
	point4d_t position;
	vector4d_t direction;
	f32 fov;
	f32 z_far;
	f32 z_near;
	matrix4x4_t matrix;
} camera_t;

typedef struct directional_light_t {
	vector4d_t direction;
	color_rgba_t diffuse;
} directional_light_t;

#define RENDERER_ATTRIBUTE_WIREFRAME_BIT 0x00000001

typedef struct renderer_t {
	u32 attributes;
	color_rgba_t clear_color;
	color_rgba_t ambient_light;
	directional_light_t directional_light;
	framebuffer_t framebuffer;
	camera_t camera;
	i32 entity_count;
	render_entity_t* entities;
	texture_t* textures;
	material_t* materials;
	color_rgba_t wireframe_color;
	matrix4x4_t projection_matrix;
} renderer_t;

// C O L O R   F U N C T I O N S ///////////////////////////////////////////////

static inline void color_swap(color_rgba_t* color1, color_rgba_t* color2) {
	swapf(&color1->r, &color2->r);
	swapf(&color1->g, &color2->g);
	swapf(&color1->b, &color2->b);
	swapf(&color1->a, &color2->a);
} // color_swap

static inline u32 color_to_rgba(color_rgba_t* color) {
	u32 r = color->r * 255.0f;
	u32 g = color->g * 255.0f;
	u32 b = color->b * 255.0f;
	u32 a = color->a * 255.0f;
	return r << 24 | g << 16 | b << 8 | a;
} // color_to_rgba

static inline u32 color_to_argb(color_rgba_t* color) {
	u32 a = color->a * 255.0f;
	u32 r = color->r * 255.0f;
	u32 g = color->g * 255.0f;
	u32 b = color->b * 255.0f;
	return a << 24 | r << 16 | g << 8 | b;
} // color_to_argb

static inline u32 color_to_abgr(color_rgba_t* color) {
	u32 a = color->a * 255.0f;
	u32 b = color->b * 255.0f;
	u32 g = color->g * 255.0f;
	u32 r = color->r * 255.0f;
	return a << 24 | b << 16 | g << 8 | r;
} // color_to_abgr

static inline void color_from_rgba(color_rgba_t* out, u32 color) {
	u8 r = color >> 24;
	u8 g = color >> 16;
	u8 b = color >> 8;
	u8 a = color;
	out->r = (f32) r / 255.0f;
	out->g = (f32) g / 255.0f;
	out->b = (f32) b / 255.0f;
	out->a = (f32) a / 255.0f;
} // color_from_rgba

static inline void color_from_argb(color_rgba_t* out, u32 color) {
	u8 a = color >> 24;
	u8 r = color >> 16;
	u8 g = color >> 8;
	u8 b = color;
	out->a = (f32) a / 255.0f;
	out->r = (f32) r / 255.0f;
	out->g = (f32) g / 255.0f;
	out->b = (f32) b / 255.0f;
} // color_from_argb

static inline u32 color_to_u32(color_rgba_t* color, image_format_t image_format)
{
	u32 c;
	switch (image_format) {
		case IMAGE_FORMAT_RGBA:
			c = color_to_rgba(color);
			break;
		case IMAGE_FORMAT_ARGB:
			c = color_to_argb(color);
			break;
		case IMAGE_FORMAT_ABGR:
			c = color_to_abgr(color);
			break;
		default:
			c = color_to_rgba(color);
			break;
	}
	return c;
} // color_to_u32

// T E X T U R E   F U N C T I O N S ///////////////////////////////////////////

static inline void texture_flip_horizontal(texture_t* tex) {
	i32 half_width = tex->width >> 1;
	for (i32 y = 0; y < tex->height; y++) {
		for (i32 x = 0; x < half_width; x++) {
			color_swap(
				&tex->data[y * tex->width + x],
				&tex->data[y * tex->width + tex->width - 1 - x]
			);
		}
	}
} // texture_flip_horizontal

static inline void texture_flip_vertical(texture_t* tex) {
	i32 half_height = tex->height >> 1;
	i32 size = tex->width * tex->height;
	i32 bottom_line = size - tex->width;
	for (i32 y = 0; y < half_height; y++) {
		for (i32 x = 0; x < tex->width; x++) {
			color_swap(
				&tex->data[y * tex->width + x],
				&tex->data[bottom_line - y * tex->width + x]
			);
		}
	}
} // texture_flip_vertical

// 2 D   D R A W I N G   F U N C T I O N S /////////////////////////////////////

static inline void clear_color(framebuffer_t* fb, color_rgba_t* color) {
	u32 c = color_to_u32(color, fb->image_format);
	for (i32 i = 0; i < fb->width * fb->height; i++)
		fb->color[i] = c;
} // clear_color

static inline void clear_depth(framebuffer_t* fb, f32 depth) {
	for (i32 i = 0; i < fb->width * fb->height; i++)
		fb->depth[i] = depth;
} // clear_depth

static inline void set_pixel(framebuffer_t* fb, i32 x, i32 y,
	color_rgba_t* color)
{
	u32 c = color_to_u32(color, fb->image_format);
	fb->color[y * fb->width + x] = c;
} // set_pixel

static inline void set_depth(framebuffer_t* fb, i32 x, i32 y, f32 depth) {
	fb->depth[y * fb->width + x] = depth;
} // set_depth

static inline f32 get_depth(framebuffer_t* fb, i32 x, i32 y) {
	if (x < 0 || x >= fb->width) return -1.0f;
	if (y < 0 || y >= fb->height) return -1.0f;
	return fb->depth[y * fb->width + x];
} // get_depth

static inline void line_draw(framebuffer_t* fb, line3d_t* line) {
	i32 x_start = line->start.position.x;
	i32 y_start = line->start.position.y;
	i32 x_end = line->end.position.x;
	i32 y_end = line->end.position.y;
	color_rgba_t color = *line->color;

	i32 dx = x_end - x_start;
	i32 dy = y_end - y_start;
	i32 x_incr = (dx >= 0) ? 1 : -1;
	i32 y_incr = (dy >= 0) ? 1 : -1;

	dx = absolute(dx);
	dy = absolute(dy);

	i32 x = x_start;
	i32 y = y_start;
	i32 error = 0;
	if (dx > dy) {
		for (i32 i = 0; i < dx; i++) {
			set_pixel(fb, x, y, &color);
			x += x_incr;
			error += dy;
			if (error > dx) {
				y += y_incr;
				error -= dx;
			}
		}
	} else {
		for (i32 i = 0; i < dy; i++) {
			set_pixel(fb, x, y, &color);
			y += y_incr;
			error += dx;
			if (error > dy) {
				x += x_incr;
				error -= dy;
			}
		}
	}
} // line_draw

static inline void triangle_stroke(framebuffer_t* fb, triangle3d_t* triangle) {
	line3d_t l1 = line3d(triangle->p1, triangle->p2, triangle->color);
	line3d_t l2 = line3d(triangle->p2, triangle->p3, triangle->color);
	line3d_t l3 = line3d(triangle->p3, triangle->p1, triangle->color);

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
	color_rgba_t color = *triangle->color;

	if (p1y > p2y) {
		swapi(&p1x, &p2x);
		swapi(&p1y, &p2y);
		swapf(&p1z, &p2z);
		swapf(&tc1u, &tc2u);
		swapf(&tc1v, &tc2v);
	}
	if (p1y > p3y) {
		swapi(&p1x, &p3x);
		swapi(&p1y, &p3y);
		swapf(&p1z, &p3z);
		swapf(&tc1u, &tc3u);
		swapf(&tc1v, &tc3v);
	}
	if (p2y > p3y) {
		swapi(&p2x, &p3x);
		swapi(&p2y, &p3y);
		swapf(&p2z, &p3z);
		swapf(&tc2u, &tc3u);
		swapf(&tc2v, &tc3v);
	}

	for (f32 y = p1y; y < p3y; y++) {
		i32 xl;
		f32 zl;
		f32 ul;
		f32 vl;
		if (y < p2y) {
			f32 y_norm = (f32) (y - p1y) / (p2y - p1y);
			xl = lerp(p1x, p2x, y_norm);
			zl = lerp(p1z, p2z, y_norm);
			ul = lerp(tc1u * p1z, tc2u * p2z, y_norm);
			vl = lerp(tc1v * p1z, tc2v * p2z, y_norm);
		} else {
			f32 y_norm = (f32) (y - p2y) / (p3y - p2y);
			xl = lerp(p2x, p3x, y_norm);
			zl = lerp(p2z, p3z, y_norm);
			ul = lerp(tc2u * p2z, tc3u * p3z, y_norm);
			vl = lerp(tc2v * p2z, tc3v * p3z, y_norm);
		}

		f32 y_norm = (f32) (y - p1y) / (p3y - p1y);
		i32 xr = lerp(p1x, p3x, y_norm);
		f32 zr = lerp(p1z, p3z, y_norm);
		f32 ur = lerp(tc1u * p1z, tc3u * p3z, y_norm);
		f32 vr = lerp(tc1v * p1z, tc3v * p3z, y_norm);

		if (xl > xr) {
			swapi(&xl, &xr);
			swapf(&zl, &zr);
			swapf(&ul, &ur);
			swapf(&vl, &vr);
		}

		for (i32 x = xl; x < xr; x++) {
			f32 x_norm = (f32) (x - xl) / (xr - xl);
			f32 z = lerp(zl, zr, x_norm);
			f32 z_inv = 1.0f / z;
			if (get_depth(fb, x, y) < z_inv) continue;

			i32 u = lerp(ul * z_inv, ur * z_inv, x_norm) * tex->width;
			i32 v = lerp(vl * z_inv, vr * z_inv, x_norm) * tex->height;

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

// 3 D   D R A W I N G   F U N C T I O N S /////////////////////////////////////

static inline void view_to_screen(point4d_t* out, point4d_t* v, i32 width,
	i32 height)
{
	f32 z_inv = 1.0f / v->z;
	out->x = (v->x * z_inv + 1.0f) * width / 2.0f;
	out->y = (-v->y * z_inv + 1.0f) * height / 2.0f;
	out->z = z_inv;
} // view_to_screen

static inline u8 cull_backface(vertex3d_t coords[], point3d_t* cam_pos) {
	point3d_t p1 = coords[0].position.xyz;
	point3d_t p2 = coords[1].position.xyz;
	point3d_t p3 = coords[2].position.xyz;

	vector3d_t l1;
	vector3d_subtract(&l1, &p1, &p2);
	vector3d_t l2;
	vector3d_subtract(&l2, &p1, &p3);
	vector3d_t n;
	vector3d_cross_product(&n, &l1, &l2);
	vector3d_t n_normalized;
	vector3d_normalize(&n_normalized, &n);
	vector3d_t p;
	vector3d_subtract(&p, &p1, cam_pos);

	f32 dot_product = vector3d_dot_product(
		&n_normalized,
		&p
	);

	if (dot_product > 0.0f) return 1;
	return 0;
} // cull_backface

static inline void camera_create_euler_matrix(matrix4x4_t* m, camera_t* c) {
	f32 x = c->direction.x;
	f32 y = c->direction.y;
	f32 z = c->direction.z;

	matrix4x4_t mt = matrix4x4_identity;
	mt.e30 = -c->position.x;
	mt.e31 = -c->position.y;
	mt.e32 = -c->position.z;

	f32 cos_x = gf_cos(x);
	f32 sin_x = -gf_sin(x);
	matrix4x4_t mrx = matrix4x4_identity;
	mrx.e11 = cos_x;
	mrx.e12 = sin_x;
	mrx.e21 = -sin_x;
	mrx.e22 = cos_x;

	f32 cos_y = gf_cos(y);
	f32 sin_y = -gf_sin(y);
	matrix4x4_t mry = matrix4x4_identity;
	mry.e00 = cos_y;
	mry.e02 = -sin_y;
	mry.e20 = sin_y;
	mry.e22 = cos_y;

	f32 cos_z = gf_cos(z);
	f32 sin_z = -gf_sin(z);
	matrix4x4_t mrz = matrix4x4_identity;
	mrz.e00 = cos_z;
	mrz.e01 = sin_z;
	mrz.e10 = -sin_z;
	mrz.e11 = cos_z;

	matrix4x4_t mr = matrix4x4_identity;
	matrix4x4_t mtemp = matrix4x4_identity;
	matrix4x4_multiply(&mtemp, &mry, &mrx);
	matrix4x4_multiply(&mr, &mtemp, &mrz);
	matrix4x4_multiply(m, &mt, &mr);
} // camera_create_euler_matrix

static inline void create_rotation_matrix(matrix4x4_t* out,
	vector4d_t* rotation)
{
	matrix4x4_t mrx = matrix4x4_identity;
	matrix4x4_t mry = matrix4x4_identity;
	matrix4x4_t mrz = matrix4x4_identity;
	matrix4x4_t temp = matrix4x4_identity;

	matrix4x4_rotation_y(&mry, rotation->y);
	matrix4x4_rotation_x(&mrx, rotation->x);
	matrix4x4_rotation_z(&mrz, rotation->z);

	matrix4x4_multiply(&temp, &mry, &mrx);
	matrix4x4_multiply(out, &temp, &mrz);
} // create_rotation_matrix

static inline void create_entity_matrix(matrix4x4_t* out,
	render_entity_t* entity)
{
	matrix4x4_t entity_rotation_matrix = matrix4x4_identity;
	create_rotation_matrix(
		&entity_rotation_matrix,
		&entity->transform.rotation
	);
	matrix4x4_t entity_scale_matrix = matrix4x4_identity;
	matrix4x4_scale(
		&entity_scale_matrix,
		entity->transform.scale.x,
		entity->transform.scale.y,
		entity->transform.scale.z
	);
	matrix4x4_t entity_translation_matrix = matrix4x4_identity;
	matrix4x4_translation(
		&entity_translation_matrix,
		entity->transform.position.x,
		entity->transform.position.y,
		entity->transform.position.z
	);
	matrix4x4_t entity_matrix_rs = matrix4x4_identity;
	matrix4x4_multiply(
		&entity_matrix_rs,
		&entity_scale_matrix,
		&entity_rotation_matrix
	);
	matrix4x4_multiply(
		out,
		&entity_matrix_rs,
		&entity_translation_matrix
	);
} // create_entity_matrix

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

static inline void render_entity_draw(renderer_t* renderer,
	render_entity_t* entity)
{
	framebuffer_t* fb = &renderer->framebuffer;
	camera_t* camera = &renderer->camera;
	matrix4x4_t entity_matrix = matrix4x4_identity;
	create_entity_matrix(
		&entity_matrix,
		entity
	);
	u32 material_index = entity->material_index;
	u32 texture_index = renderer->materials[material_index].texture_index;
	f32 aspect = (f32) fb->height / (f32) fb->width;
	f32 fov_rad = 1.0f / tan(camera->fov * 0.5f);
	f32 aspect_fov = aspect * fov_rad;
	for (u32 i = 0; i < entity->face_count; i++) {
		face3d_t* face = &entity->faces[i];
		u32 index_count = face->index_count;
		color_rgba_t color = *entity->faces[i].color;
		vertex3d_t local_coords[index_count];
		for (u32 j = 0; j < index_count; j++) {
			local_coords[j].position = entity->vertices[
				face->indices[j].position
			];
			local_coords[j].texcoord = entity->texcoords[
				face->indices[j].texcoord
			];
		}

		vertex3d_t world_coords[index_count];
		for (u32 j = 0; j < index_count; j++) {
			vector4d_multiply_matrix4x4(
				&world_coords[j].position,
				&local_coords[j].position,
				&entity_matrix
			);
			world_coords[j].texcoord = local_coords[j].texcoord;
		}

		vertex3d_t camera_coords[index_count];
		for (u32 j = 0; j < index_count; j++) {
			vector4d_multiply_matrix4x4(
				&camera_coords[j].position,
				&world_coords[j].position,
				&camera->matrix
			);
			camera_coords[j].texcoord = world_coords[j].texcoord;
		}

		point3d_t cam_pos = point3d(0.0f, 0.0f, 0.0f);
		if (cull_backface(camera_coords, &cam_pos)) continue;

		// TODO: Clipping works only with 90° field of view
		i32 clip_plane_count = 6;
		plane3d_t clip_planes[clip_plane_count];
		// Near Plane
		clip_planes[0] = plane3d(
			camera->z_near,
			vector3d(0.0f, 0.0f, 1.0f)
		);
		// Far Plane
		clip_planes[1] = plane3d(
			-camera->z_far,
			vector3d(0.0f, 0.0f, -1.0f)
		);
		// Left Plane
		vector3d_t clip_dir = vector3d(-aspect_fov, 0.0f, fov_rad);
		vector3d_normalize(&clip_dir, &clip_dir);
		clip_planes[2] = plane3d(
			0,
			clip_dir
		);
		// Right Plane
		clip_dir = vector3d(aspect_fov, 0.0f, fov_rad);
		vector3d_normalize(&clip_dir, &clip_dir);
		clip_planes[3] = plane3d(
			0,
			clip_dir
		);
		// Top Plane
		clip_dir = vector3d(0.0f, -fov_rad, fov_rad);
		vector3d_normalize(&clip_dir, &clip_dir);
		clip_planes[4] = plane3d(
			0,
			clip_dir
		);
		// Bottom Plane
		clip_dir = vector3d(0.0f, fov_rad, fov_rad);
		vector3d_normalize(&clip_dir, &clip_dir);
		clip_planes[5] = plane3d(
			0,
			clip_dir
		);

		vertex3d_t clip_coords[clip_plane_count][index_count * 2];
		i32 clip_coords_count = triangle3d_clip(
			clip_coords[0],
			camera_coords,
			&clip_planes[0],
			index_count
		);
		for (i32 j = 1; j < clip_plane_count; j++) {
			clip_coords_count = triangle3d_clip(
				clip_coords[j],
				clip_coords[j-1],
				&clip_planes[j],
				clip_coords_count
			);
		}
		
		if (clip_coords_count < 3) continue;
		
		vertex3d_t projected_coords[clip_coords_count];
		for (i32 j = 0; j < clip_coords_count; j++) {
			vector4d_multiply_matrix4x4(
				&projected_coords[j].position,
				&clip_coords[clip_plane_count-1][j].position,
				&renderer->projection_matrix
			);
			projected_coords[j].texcoord = 
				clip_coords[clip_plane_count-1][j].texcoord;
		}

		vertex3d_t screen_coords[clip_coords_count];
		for (i32 j = 0; j < clip_coords_count; j++) {
			view_to_screen(
				&screen_coords[j].position,
				&projected_coords[j].position,
				fb->width,
				fb->height
			);
			screen_coords[j].texcoord = projected_coords[j].texcoord;
		}

		// Shading
		point3d_t p1 = world_coords[0].position.xyz;
		point3d_t p2 = world_coords[1].position.xyz;
		point3d_t p3 = world_coords[2].position.xyz;

		vector3d_t l1;
		vector3d_subtract(&l1, &p1, &p2);
		vector3d_t l2;
		vector3d_subtract(&l2, &p1, &p3);
		vector3d_t n;
		vector3d_cross_product(&n, &l1, &l2);
		vector3d_negate(&n, &n);
		vector3d_normalize(&n, &n);
		vector3d_t dlight_direction;
		vector3d_normalize(
			&dlight_direction,
			&renderer->directional_light.direction.xyz
		);

		f32 intensity = vector3d_dot_product(
			&dlight_direction,
			&n
		);
		intensity = clamp(intensity, 0.0f, 1.0f);

		color_rgba_t diffuse;
		vector3d_multiply_float(
			&diffuse.rgb,
			&renderer->directional_light.diffuse.rgb,
			intensity
		);

		color_rgba_t ambient_light;
		vector3d_add(
			&ambient_light.rgb,
			&renderer->ambient_light.rgb,
			&diffuse.rgb
		);
		vector3d_multiply(&color.rgb, &color.rgb, &ambient_light.rgb);
		vector4d_clamp(&color.rgba, &color.rgba, 0.0f, 1.0f);

		for (i32 j = 0; j < clip_coords_count - 1; j++) {
			triangle3d_t triangle = triangle3d(
				screen_coords[0],
				screen_coords[j],
				screen_coords[j+1],
				&renderer->textures[texture_index],
				&color
			);

			triangle_fill(fb, &triangle);
			i32 wireframe = renderer->attributes &
				RENDERER_ATTRIBUTE_WIREFRAME_BIT;
			if (wireframe) {
				triangle.color = &renderer->wireframe_color;
				triangle_stroke(fb, &triangle);
			}
		}
	}
} // render_entity_draw

// P U B L I C   F U N C T I O N S /////////////////////////////////////////////

static inline void renderer_init(renderer_t* renderer) {
	framebuffer_t* fb = &renderer->framebuffer;

	camera_t* camera = &renderer->camera;
	renderer->projection_matrix = matrix4x4_identity;
	matrix4x4_projection(&renderer->projection_matrix, camera->z_near,
		camera->z_far, camera->fov, fb->width, fb->height);
} // renderer_init

static inline void renderer_loop(renderer_t* renderer) {
	framebuffer_t* fb = &renderer->framebuffer;

	clear_color(fb, &renderer->clear_color);
	clear_depth(fb, 1000.0f);

	camera_t* camera = &renderer->camera;
	camera_create_euler_matrix(&camera->matrix, camera);

	for (i32 i = 0; i < renderer->entity_count; i++) {
		render_entity_t* entity = &renderer->entities[i];

		render_entity_draw(renderer, entity);
	}
} // renderer_loop

#endif // RENDERER_H
