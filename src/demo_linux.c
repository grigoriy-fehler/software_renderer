#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "lookup_tables.c"
#include "maths.h"
#include "renderer.h"

// D E F I N E S ///////////////////////////////////////////////////////////////

#define WINDOW_NAME "Software Renderer"
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

#define KEYCODE_ESCAPE 9
#define KEYCODE_Q 24
#define KEYCODE_W 25
#define KEYCODE_E 26
#define KEYCODE_A 38
#define KEYCODE_S 39
#define KEYCODE_D 40

#define KEYCODE_UP 111
#define KEYCODE_DOWN 116
#define KEYCODE_LEFT 113
#define KEYCODE_RIGHT 114

#define RENDER_ENTITY_COUNT 5
#define TEXTURE_COUNT 5
#define MATERIAL_COUNT TEXTURE_COUNT

// S T R U C T   D E F I N I T I O N S /////////////////////////////////////////

typedef struct platform_state_t {
	xcb_connection_t* connection;
	xcb_window_t window;
	const xcb_setup_t* setup;
	xcb_screen_t* screen;
	xcb_generic_event_t* event;
	xcb_client_message_event_t* cm_event;
	xcb_atom_t wm_protocols;
	xcb_atom_t wm_window_delete;
	xcb_image_t* image;
	xcb_pixmap_t pixmap;
	xcb_gcontext_t gc;
	int32_t width;
	int32_t height;
	int32_t quit;
} platform_state_t;

// G L O B A L   V A R I A B L E S /////////////////////////////////////////////

static color_rgba_t color_black = color_rgba(0.086274f, 0.086274f, 0.086274f, 1.0f);
static color_rgba_t color_white = color_rgba(0.941176f, 0.941176f, 0.941176f, 1.0f);
static color_rgba_t color_red = color_rgba(0.819607f, 0.309803f, 0.172549f, 1.0f);

static platform_state_t ps = { 0 };
static renderer_t renderer = { 0 };
static render_entity_t entities[RENDER_ENTITY_COUNT] = { 0 };
static texture_t textures[TEXTURE_COUNT] = { 0 };
static material_t materials[MATERIAL_COUNT] = { 0 };

static int key_q = 0;
static int key_w = 0;
static int key_e = 0;
static int key_a = 0;
static int key_s = 0;
static int key_d = 0;

static int key_up = 0;
static int key_down = 0;
static int key_left = 0;
static int key_right = 0;

static const char* obj_paths[RENDER_ENTITY_COUNT] = {
	"assets/fortress.obj",
	"assets/fortress_environment.obj",
	"assets/fortress_sand.obj",
	"assets/fortress_sea.obj",
	"assets/fortress_sky.obj"
};

static const char* texture_paths[TEXTURE_COUNT] = {
	"assets/fortress_diffuse.tga",
	"assets/fortress_environment_diffuse.tga",
	"assets/fortress_sand_diffuse.tga",
	"assets/fortress_sea_diffuse.tga",
	"assets/fortress_sky_diffuse.tga"
};

// F U N C T I O N   D E C L A R A T I O N S ///////////////////////////////////

static inline void quit();

// D Y N A M I C   A R R A Y ///////////////////////////////////////////////////

typedef struct darray_header_t {
	int size;
	int capacity;
} darray_header_t;

#define darray(type) type*
#define darray_init(array, size) darray_init_impl((int) sizeof *array, (int) size)
#define darray_head(array) (((darray_header_t *) array) - 1)
#define darray_size(array) (array == NULL ? 0 : darray_head(array)->size)
#define darray_capacity(array) (array == NULL ? 0 : darray_head(array)->capacity)
#define darray_empty(array) (darray_size(array) == 0)
#define darray_full(array) (darray_size(array) == darray_capacity(array))
#define darray_clear(array) darray_head(array)->size = 0
#define darray_free(array) darray_free_impl(array)
#define darray_resize(array, new_size) darray_resize_impl(array, (int) new_size, (int) sizeof *array)
#define darray_push(array, item) \
	do { \
		darray_header_t* head = darray_head(array); \
		if (darray_full(array)) { \
			array = darray_resize(array, head->capacity * 2); \
			head = darray_head(array); \
		} \
		*(array + head->size) = item; \
		head->size++; \
	} while (0)
// TODO: Return the removed value
#define darray_pop(array) (darray_head(array)->size -= 1)

static inline void* darray_init_impl(int item_size, int arr_size) {
	darray_header_t* head = malloc(sizeof(darray_header_t) + arr_size * item_size);
	head->size = 0;
	head->capacity = arr_size;
	return ++head;
} // darray_init_impl

static inline void darray_free_impl(void* array) {
	free(darray_head(array));
} // darray_free_impl

static inline void* darray_resize_impl(void* array, int new_size, int item_size) {
	if (array == NULL)
		return darray_init_impl(item_size, new_size);

	darray_header_t* head = darray_head(array);
	head = realloc(head, sizeof(darray_header_t) + new_size * item_size);
	head->capacity = new_size;
	return ++head;
} // darray_resize_impl

// F I L E   F U N C T I O N S /////////////////////////////////////////////////

typedef struct file_info_t {
	u8* buffer;
	size_t buffer_size;
	size_t bytes_read;
} file_info_t;

static inline file_info_t file_read_sync(const char* filepath) {
	file_info_t file_info = { 0 };
	FILE* file = fopen(filepath, "rb");
	if (!file) {
		printf("Could not Load File: %s!\n", filepath);
		return file_info;
	}

	fseek(file, 0L, SEEK_END);
	size_t buffer_size = ftell(file);
	u8* buffer = malloc(sizeof(u8) * buffer_size);
	rewind(file);

	size_t bytes_read = fread(buffer, 1, buffer_size, file);

	i32 error = ferror(file);

	fclose(file);

	if (error) {
		printf("Error Reading File!\n\tError Code: %d!\n", error);
		return file_info;
	}

	file_info.buffer = buffer;
	file_info.buffer_size = buffer_size;
	file_info.bytes_read = bytes_read;

	return file_info;
} // file_read_sync

// T G A   F U N C T I O N S ///////////////////////////////////////////////////

#define TGA_VERTICAL_FLIP_BIT 0x20
#define TGA_HORIZONTAL_FLIP_BIT 0x10

#pragma pack(push, 1)
typedef struct tga_header_t {
	u8 id_length;
	u8 color_map_type;
	u8 image_type;
	u16 color_map_start;
	u16 color_map_length;
	u8 color_map_depth;
	u16 x_origin;
	u16 y_origin;
	u16 width;
	u16 height;
	u8 bits_per_pixel;
	u8 image_descriptor;
} tga_header_t;
#pragma pack(pop)

// TODO: Add support for missing image types
// NOTE: Only uncompressed tga files supported
static inline texture_t* texture_load_from_tga(const char* filepath) {
	printf("Loading Image File: %s\n", filepath);

	texture_t* tex = malloc(sizeof(texture_t));

	file_info_t file = file_read_sync(filepath);
	tga_header_t* header = (tga_header_t *) file.buffer;
	printf("\tID Length: %d\n", header->id_length);
	printf("\tColor Map Type: %d\n", header->color_map_type);
	printf("\tImage Type: %d\n", header->image_type);
	printf("\tColor Map Start: %d\n", header->color_map_start);
	printf("\tColor Map Length: %d\n", header->color_map_length);
	printf("\tColor Map Depth: %d\n", header->color_map_depth);
	printf("\tX Origin: %d\n", header->x_origin);
	printf("\tY Origin: %d\n", header->y_origin);
	printf("\tWidth: %d\n", header->width);
	printf("\tHeight: %d\n", header->height);
	printf("\tBits per Pixel: %d\n", header->bits_per_pixel);
	printf("\tImage Descriptor: %d\n", header->image_descriptor);

	tex->width = header->width;
	tex->height = header->height;
	i32 size = tex->width * tex->height;
	tex->data = malloc(sizeof *tex->data * size);
	i32 bytes_per_pixel = header->bits_per_pixel >> 3;
	u8* image_data = file.buffer + sizeof(tga_header_t);

	if (header->image_type == 2 || header->image_type == 3) {
		for (i32 i = 0; i < size; i++) {
			if (bytes_per_pixel == 1) {
				tex->data[i].b = image_data[i * bytes_per_pixel] / 255.0f;
				tex->data[i].g = image_data[i * bytes_per_pixel] / 255.0f;
				tex->data[i].r = image_data[i * bytes_per_pixel] / 255.0f;
			} else {
				tex->data[i].b = image_data[i * bytes_per_pixel] / 255.0f;
				tex->data[i].g = image_data[i * bytes_per_pixel + 1] / 255.0f;
				tex->data[i].r = image_data[i * bytes_per_pixel + 2] / 255.0f;
			}
			if (bytes_per_pixel == 4) {
				tex->data[i].a = image_data[i * bytes_per_pixel + 3] / 255.0f;
			} else {
				tex->data[i].a = 1.0f;
			}
		}
	}

	u32 vertical_flip = header->image_descriptor & TGA_VERTICAL_FLIP_BIT;
	u32 horizontal_flip = header->image_descriptor & TGA_HORIZONTAL_FLIP_BIT;
	if (horizontal_flip) {
		texture_flip_horizontal(tex);
	}
	if (vertical_flip) {
		texture_flip_vertical(tex);
	}

	free(file.buffer);

	printf("Loading Image File %s Finished\n", filepath);

	return tex;
} // texture_load_from_tga

// O B J   F U N C T I O N S ///////////////////////////////////////////////////

static inline render_entity_t* render_entity_load_from_obj(const char* filepath,
	transform4d_t* transform, i32 material_index, color_rgba_t* color)
{
	printf("Loading OBJ File: %s\n", filepath);

	render_entity_t* entity = malloc(sizeof(render_entity_t));
	darray(point4d_t) vertices = darray_init(vertices, 4);
	darray(point2d_t) texcoords = darray_init(texcoords, 4);
	darray(index3d_t) indices = darray_init(indices, 2);
	darray(face3d_t) faces = darray_init(faces, 2);

	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		printf("Could not Load File: %s!\n", filepath);
		return NULL;
	}

	char c = fgetc(file);
	while (!feof(file)) {
		point4d_t v = point4d(0.0f, 0.0f, 0.0f);
		point2d_t t = point2d(0.0f, 0.0f);

		if (c == 'v') {
			c = fgetc(file);
			if (c == ' ') { // Vertex
				i32 num = fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
				if (num != 3)
					fprintf(stderr, "Error loading file!\n");
				darray_push(vertices, v);
			} else if (c == 't') { // Texture Coord
				i32 num = fscanf(file, " %f %f\n", &t.u, &t.v);
				if (num != 2)
					fprintf(stderr, "Error loading file!\n");
				darray_push(texcoords, t);
			}
		} else if (c == 'f' && (c = fgetc(file)) == ' ') {
			darray_clear(indices);
			while (c != '\n') {
				index3d_t index = { 0 };
				i32 num = fscanf(file, "%d/%d", &index.position, &index.texcoord);
				if (num != 2)
					fprintf(stderr, "Error loading file!\n");
				// NOTE: In OBJ indices start with 1;
				//       In renderer indices start with 0
				index.position--;
				index.texcoord--;
				darray_push(indices, index);
				c = fgetc(file);
			}
			face3d_t face = { 0 };
			face.color = color;
			face.index_count = darray_size(indices);
			int index_array_size = sizeof *indices * face.index_count;
			face.indices = malloc(index_array_size);
			memcpy(face.indices, indices, index_array_size);
			darray_push(faces, face);
		} else {
			while (c != '\n') {
				c = fgetc(file);
			}
		}
		c = fgetc(file);
	}
	fclose(file);

	darray_free(indices);

	printf("\tVertex Count: %d\n", darray_size(vertices));
	entity->vertex_count = darray_size(vertices);
	int vertex_array_size = sizeof *entity->vertices * entity->vertex_count;
	entity->vertices = malloc(vertex_array_size);
	memcpy(entity->vertices, vertices, vertex_array_size);
	darray_free(vertices);

	printf("\tTexcoord Count: %d\n", darray_size(texcoords));
	entity->texcoord_count = darray_size(texcoords);
	int texcoord_array_size = sizeof *entity->texcoords * entity->texcoord_count;
	entity->texcoords = malloc(texcoord_array_size);
	memcpy(entity->texcoords, texcoords, texcoord_array_size);
	darray_free(texcoords);

	printf("\tFace Count: %d\n", darray_size(faces));
	entity->face_count = darray_size(faces);
	int face_array_size = sizeof *entity->faces * entity->face_count;
	entity->faces = malloc(face_array_size);
	memcpy(entity->faces, faces, face_array_size);
	darray_free(faces);

	entity->material_index = material_index;
	entity->transform = *transform;

	printf("Loading OBJ File %s Finished\n", filepath);
	return entity;
} // render_entity_load_from_obj

static inline void render_entity_free(render_entity_t* entity) {
	free(entity->vertices);
	free(entity->texcoords);
	for (u32 i = 0; i < entity->face_count; i++) {
		free(entity->faces[i].indices);
	}
	free(entity->faces);
} // render_entity_free

// R E N D E R E R   F U N C T I O N S /////////////////////////////////////////

static inline void renderer_software_on_resize(int32_t width, int32_t height) {
	framebuffer_t* framebuffer = &renderer.framebuffer;

	// NOTE: 'xcb_image_destroy' frees 'framebuffer->color'
	xcb_image_destroy(ps.image);
	xcb_free_gc(ps.connection, ps.gc);
	xcb_free_pixmap(ps.connection, ps.pixmap);

	free(framebuffer->depth);

	int32_t size = width * height;
	framebuffer->width = width;
	framebuffer->height = height;
	framebuffer->color = malloc(sizeof *framebuffer->color * size);
	framebuffer->depth = malloc(sizeof *framebuffer->depth * size);

	uint32_t gc_mask = XCB_GC_FOREGROUND;
	uint32_t gc_value[] = { ps.screen->white_pixel };
	ps.gc = xcb_generate_id(ps.connection);
	ps.pixmap = xcb_generate_id(ps.connection);

	xcb_create_pixmap(ps.connection, ps.screen->root_depth, ps.pixmap,
		ps.window, width, height);
	xcb_create_gc(ps.connection, ps.gc, ps.pixmap, gc_mask, gc_value);

	ps.image = xcb_image_create_native(ps.connection, width, height,
		XCB_IMAGE_FORMAT_Z_PIXMAP, ps.screen->root_depth,
		(void *) framebuffer->color, size * sizeof(uint32_t),
		(uint8_t *) framebuffer->color);
		
	framebuffer_t* fb = &renderer.framebuffer;
	matrix4x4_projection(&renderer.projection_matrix,
		renderer.camera.z_near, renderer.camera.z_far,
		renderer.camera.fov, fb->width, fb->height);
} // renderer_software_on_resize

static inline void renderer_software_init(int32_t width, int32_t height) {
	framebuffer_t* framebuffer = &renderer.framebuffer;

	int32_t size = width * height;
	framebuffer->width = width;
	framebuffer->height = height;
	framebuffer->color = malloc(sizeof *framebuffer->color * size);
	framebuffer->depth = malloc(sizeof *framebuffer->depth * size);

	renderer.clear_color = color_red;

	uint32_t gc_mask = XCB_GC_FOREGROUND;
	uint32_t gc_value[] = { ps.screen->white_pixel };
	ps.gc = xcb_generate_id(ps.connection);
	ps.pixmap = xcb_generate_id(ps.connection);

	xcb_create_pixmap(ps.connection, ps.screen->root_depth, ps.pixmap,
		ps.window, width, height);
	xcb_create_gc(ps.connection, ps.gc, ps.pixmap, gc_mask, gc_value);

	ps.image = xcb_image_create_native(ps.connection, width, height,
		XCB_IMAGE_FORMAT_Z_PIXMAP, ps.screen->root_depth,
		(void *) framebuffer->color, size * sizeof(uint32_t),
		(uint8_t *) framebuffer->color);
} // renderer_software_init

static inline void renderer_software_shut() {
	framebuffer_t* framebuffer = &renderer.framebuffer;

	// NOTE: 'xcb_image_destroy' frees 'framebuffer->color'
	xcb_image_destroy(ps.image);
	xcb_free_gc(ps.connection, ps.gc);
	xcb_free_pixmap(ps.connection, ps.pixmap);

	free(framebuffer->depth);
} // renderer_software_shut

// X C B   W I N D O W   F U N C T I O N S /////////////////////////////////////

static inline void window_set_keyrepeat(xcb_auto_repeat_mode_t mode) {
	uint32_t keyrepeat_mask = XCB_KB_AUTO_REPEAT_MODE;
	uint32_t keyrepeat_value[] = { mode };
	xcb_void_cookie_t keyboard_control_cookie = 
		xcb_change_keyboard_control_checked(ps.connection,
		keyrepeat_mask, keyrepeat_value);
	xcb_generic_error_t* error = xcb_request_check(ps.connection,
		keyboard_control_cookie);
	if (error)
		fprintf(stderr, "Error in XCB Change Keyboard Auto Repeate!\n");
} // window_set_keyrepeat

static inline void window_handle_key(int32_t keycode, int32_t pressed) {
	switch (keycode) {
		case KEYCODE_Q:
			key_q = pressed;
			break;
		case KEYCODE_W:
			key_w = pressed;
			break;
		case KEYCODE_E:
			key_e = pressed;
			break;
		case KEYCODE_A:
			key_a = pressed;
			break;
		case KEYCODE_S:
			key_s = pressed;
			break;
		case KEYCODE_D:
			key_d = pressed;
			break;
		case KEYCODE_UP:
			key_up = pressed;
			break;
		case KEYCODE_DOWN:
			key_down = pressed;
			break;
		case KEYCODE_LEFT:
			key_left = pressed;
			break;
		case KEYCODE_RIGHT:
			key_right = pressed;
			break;
		case KEYCODE_ESCAPE:
			ps.quit = 1;
			break;
		default: break;
	}
} // window_handle_key

static inline void window_init() {
	int32_t error = 0;
	int32_t num_screens = 0;
	ps.connection = xcb_connect(NULL, &num_screens);
	if ((error = xcb_connection_has_error(ps.connection))) {
		fprintf(stderr, "XCB Connection Error: %d\n", error);
		xcb_disconnect(ps.connection);
		quit();
	}

	ps.setup = xcb_get_setup(ps.connection);
	xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(ps.setup);
	for (int32_t i = 0; i < num_screens; i++)
		xcb_screen_next(&iterator);
	ps.screen = iterator.data;

	uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t value_list[] = {
		ps.screen->black_pixel,
		XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
		XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
		XCB_EVENT_MASK_STRUCTURE_NOTIFY
	};
	ps.window = xcb_generate_id(ps.connection);

	xcb_create_window(ps.connection, XCB_COPY_FROM_PARENT, ps.window,
		ps.screen->root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, ps.screen->root_visual,
		event_mask, value_list);

	xcb_change_property(ps.connection, XCB_PROP_MODE_REPLACE, ps.window,
		XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
		strlen(WINDOW_NAME), WINDOW_NAME);

	renderer_software_init(WINDOW_WIDTH, WINDOW_HEIGHT);

	xcb_intern_atom_cookie_t wm_cookie_delete = xcb_intern_atom(
		ps.connection, 0, strlen("WM_DELETE_WINDOW"),
		"WM_DELETE_WINDOW");
	xcb_intern_atom_cookie_t wm_cookie_protocols = xcb_intern_atom(
		ps.connection, 0, strlen("WM_PROTOCOLS"),
		"WM_PROTOCOLS");
	xcb_intern_atom_reply_t* wm_reply_delete = xcb_intern_atom_reply(
		ps.connection, wm_cookie_delete, NULL);
	xcb_intern_atom_reply_t* wm_reply_protocols = xcb_intern_atom_reply(
		ps.connection, wm_cookie_protocols, NULL);
	ps.wm_window_delete = wm_reply_delete->atom;
	ps.wm_protocols = wm_reply_protocols->atom;

	xcb_change_property(ps.connection, XCB_PROP_MODE_REPLACE, ps.window,
		wm_reply_protocols->atom, 4, 32, 1, &wm_reply_delete->atom);

	window_set_keyrepeat(XCB_AUTO_REPEAT_MODE_OFF);

	xcb_map_window(ps.connection, ps.window);
	xcb_flush(ps.connection);
} // window_init

static inline void window_handle_event() {
	uint32_t pressed;
	xcb_configure_notify_event_t* configure_event;
	switch (ps.event->response_type & ~0x80) {
		case XCB_KEY_PRESS:
		case XCB_KEY_RELEASE:
			pressed = (ps.event->response_type == XCB_KEY_PRESS);

			xcb_key_press_event_t* kb_event =
				(xcb_key_press_event_t *) ps.event;
			xcb_keycode_t keycode = kb_event->detail;
			window_handle_key(keycode, pressed);
			break;
		case XCB_BUTTON_PRESS:
		case XCB_BUTTON_RELEASE:
			break;
		case XCB_MOTION_NOTIFY:
			break;
		case XCB_CONFIGURE_NOTIFY:
			configure_event = (xcb_configure_notify_event_t *)
				ps.event;
			ps.width = configure_event->width;
			ps.height = configure_event->height;
			renderer_software_on_resize(ps.width, ps.height);
			break;
		case XCB_CLIENT_MESSAGE:
			ps.cm_event = (xcb_client_message_event_t *) ps.event;
			if (ps.cm_event->data.data32[0] == ps.wm_window_delete)
				ps.quit = 1;
			break;
		default: break;
	}
} // window_handle_event

static inline void window_shut() {
	window_set_keyrepeat(XCB_AUTO_REPEAT_MODE_DEFAULT);

	renderer_software_shut();

	xcb_destroy_window(ps.connection, ps.window);
	xcb_disconnect(ps.connection);
} // window_shut

// M A I N   F U N C T I O N ///////////////////////////////////////////////////

static inline void init() {
	framebuffer_t* fb = &renderer.framebuffer;

	renderer.clear_color = color_red;
	renderer.entity_count = RENDER_ENTITY_COUNT;

	renderer.ambient_light = color_rgba(
		0.4f, 0.4f, 0.4f, 1.0f
	);
	renderer.directional_light.direction = vector4d(
		1.0f, -1.0f, 1.0f
	);
	renderer.directional_light.diffuse = color_rgba(
		1.0f, 1.0f, 1.0f, 1.0f
	);

	renderer.camera.position = point4d(0.0f, 38.2f, -56.2f),
	renderer.camera.direction = vector4d(25.0f, 0.0f, 0.0f),
	renderer.camera.fov = 90.0f;
	renderer.camera.z_near = 0.5f;
	renderer.camera.z_far = 2000.0f;
	
	transform4d_t transform = transform4d(
		point4d(0.0f, 0.0f, 0.0f),
		vector4d(0.0f, 0.0f, 0.0f),
		vector4d(1.0f, 1.0f, 1.0f)
	);
	for (int i = 0; i < RENDER_ENTITY_COUNT; i++) {
		entities[i] = *render_entity_load_from_obj(obj_paths[i],
			&transform, i, &color_white);
	}
	renderer.entities = entities;

	for (int i = 0; i < TEXTURE_COUNT; i++) {
		textures[i] = *texture_load_from_tga(texture_paths[i]);
	}
	renderer.textures = textures;

	materials[0].texture_index = 0;
	materials[1].texture_index = 1;
	materials[2].texture_index = 2;
	materials[3].texture_index = 3;
	materials[4].texture_index = 4;
	renderer.materials = materials;

	renderer.wireframe_color = color_black;

	matrix4x4_projection(&renderer.projection_matrix,
		renderer.camera.z_near, renderer.camera.z_far,
		renderer.camera.fov, fb->width, fb->height);
	
	renderer.framebuffer.image_format = IMAGE_FORMAT_ARGB;

	renderer_init(&renderer);
} // init

static inline void loop(double dt) {
	for (i32 i = 0; i < RENDER_ENTITY_COUNT; i++) {
		render_entity_t* entity = &renderer.entities[i];

		f32 rot_speed = 32.0f;
		i32 rot_x = key_right - key_left;
		i32 rot_y = key_up - key_down;
		entity->transform.rotation.y -= rot_x * rot_speed * dt;
		entity->transform.rotation.x -= rot_y * rot_speed * dt;
		entity->transform.rotation.x = clamp(
			entity->transform.rotation.x,
			-40.0f, 25.0f
		);
	}
} // loop

int main() {
	window_init();
	
	init();

	renderer_init(&renderer);

	double counter = 0.0f;
	clock_t time_previous = clock();
	while (!ps.quit) {
		clock_t time_current = clock();
		double dt = difftime(time_current, time_previous) /
			CLOCKS_PER_SEC;

		counter += dt;
		if (counter > 1.0f) {
			printf("FPS: %f\tMS: %f\n", 1 / dt, dt * 1000);
			counter -= 1.0f;
		}

		while ((ps.event = xcb_poll_for_event(ps.connection)))
			window_handle_event();

		loop(dt);
		renderer_loop(&renderer);

		xcb_image_put(ps.connection, ps.pixmap, ps.gc, ps.image, 0, 0, 0);
		xcb_copy_area(ps.connection, ps.pixmap, ps.window, ps.gc,
			0, 0, 0, 0, ps.width, ps.height);

		time_previous = time_current;
	}

	window_shut();

	return 0;
} // main

static inline void quit() {
	window_set_keyrepeat(XCB_AUTO_REPEAT_MODE_DEFAULT);
	exit(1);
} // quit
