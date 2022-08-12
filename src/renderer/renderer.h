#ifndef RENDERER_H
#define RENDERER_H

#include "../math/mathlib.h"

#include "camera.h"
#include "color_rgba.h"
#include "entity3d.h"
#include "framebuffer.h"
#include "light_directional.h"
#include "material3d.h"
#include "texture.h"
#include "polygon3d.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define RENDERER_ATTRIBUTE_WIREFRAME_BIT 0x0001
#define RENDERER_ATTRIBUTE_SHADED_BIT 0x0002

// S T R U C T S ///////////////////////////////////////////////////////////////

typedef struct renderer_t {
	u32 attributes;
	color_rgba_t clear_color;
	color_rgba_t ambient_light;
	directional_light_t directional_light;
	framebuffer_t framebuffer;
	camera_t camera;
	i32 entity_count;
	render_entity3d_t* entities;
	texture_t* textures;
	material3d_t* materials;
	color_rgba_t wireframe_color;
	matrix4x4_t projection_matrix;
} renderer_t;

// F U N C T I O N S ///////////////////////////////////////////////////////////

static inline void render_entity_draw(renderer_t* renderer,
	render_entity3d_t* entity)
{
	framebuffer_t* fb = &renderer->framebuffer;
	camera_t* camera = &renderer->camera;
	matrix4x4_t entity_matrix = matrix4x4_identity;
	create_entity_matrix(&entity_matrix, entity);
	u32 material_index = entity->material_index;
	u32 texture_index = renderer->materials[material_index].texture_index;

	for (u32 i = 0; i < entity->face_count; i++) {
		// Setup polygon
		face3d_t* face = &entity->faces[i];
		u32 index_count = face->index_count;
		vertex3d_t local_coords[index_count];
		polygon3d_t poly_local = polygon3d(index_count, local_coords);
		for (u32 j = 0; j < index_count; j++) {
			local_coords[j].position = entity->vertices[
				face->indices[j].position
			];
			local_coords[j].texcoord = entity->texcoords[
				face->indices[j].texcoord
			];
		}

		// Transform local -> world
		vertex3d_t world_coords[index_count];
		polygon3d_t poly_world = polygon3d(index_count, world_coords);
		polygon3d_transform(&poly_world, &poly_local, &entity_matrix);

		// Transform world -> camera
		vertex3d_t camera_coords[index_count];
		polygon3d_t poly_camera = polygon3d(index_count, camera_coords);
		polygon3d_transform(&poly_camera, &poly_world, &camera->matrix);

		// Backface culling
		point3d_t cam_pos = point3d(0.0f, 0.0f, 0.0f);
		if (polygon3d_cull(&poly_camera, &cam_pos)) continue;

		// Polygon Clipping
		vertex3d_t clip_coords[CLIPPING_PLANES_COUNT][index_count * 2];
		i32 clip_coords_count = triangle3d_clip(
			clip_coords[0],
			camera_coords,
			&camera->clipping_planes[0],
			index_count
		);
		for (i32 j = 1; j < CLIPPING_PLANES_COUNT; j++) {
			clip_coords_count = triangle3d_clip(
				clip_coords[j],
				clip_coords[j-1],
				&camera->clipping_planes[j],
				clip_coords_count
			);
		}
		
		if (clip_coords_count < 3) continue;
		
		polygon3d_t poly_clipped = polygon3d(
			clip_coords_count,
			clip_coords[CLIPPING_PLANES_COUNT - 1]
		);
		
		// Transform clipped -> projected
		vertex3d_t projected_coords[clip_coords_count];
		polygon3d_t poly_projected = polygon3d(
			clip_coords_count,
			projected_coords
		);
		polygon3d_transform(
			&poly_projected,
			&poly_clipped,
			&renderer->projection_matrix
		);

		// Transform projected -> screen
		vertex3d_t screen_coords[clip_coords_count];
		polygon3d_t poly_screen = polygon3d(
			clip_coords_count,
			screen_coords
		);
		polygon3d_project_to_screen(
			&poly_screen,
			&poly_projected,
			fb->width,
			fb->height
		);

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
		color_rgba_t color = color_rgba(1.0f, 1.0f, 1.0f, 1.0f);
		vector3d_multiply(&color.rgb, &color.rgb, &ambient_light.rgb);
		vector4d_clamp(&color.rgba, &color.rgba, 0.0f, 1.0f);

		// Setup and draw triangles
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
	camera_create_clipping_planes(camera, fb);

	for (i32 i = 0; i < renderer->entity_count; i++) {
		render_entity3d_t* entity = &renderer->entities[i];

		render_entity_draw(renderer, entity);
	}
} // renderer_loop

#endif // RENDERER_H
