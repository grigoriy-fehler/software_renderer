#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "math/maths.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "graphics.h"
#include "model.h"

Context* ctx;
Scene* scn;
Camera* camera;

Model* model;

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;
SDL_Texture* sdl_texture;

uint8_t running = 1;

float rot_speed = 4.0f;

float mov_z = 3.0f;
float mov_speed = 2.0f;

const uint8_t* key_state = NULL;

void init() {
	ctx = ctx_create(640, 480);
	
	model = model_load_obj("../assets/cube.obj");
	
	Vector3f camera_pos = { 0, 0, 3.0f };
	camera = cam_create(90, 1.0f, 100.0f, camera_pos);
	scn = scn_create(model, 1, camera);
}

void update(double delta) {
	float rot_x = 0.0f;
	float rot_y = 0.0f;
	float rot_z = 0.0f;

	if (key_state[SDL_SCANCODE_Q])
		rot_x += rot_speed * delta;
	if (key_state[SDL_SCANCODE_W])
		rot_y += rot_speed * delta;
	if (key_state[SDL_SCANCODE_E])
		rot_z += rot_speed * delta;
	if (key_state[SDL_SCANCODE_A])
		rot_x -= rot_speed * delta;
	if (key_state[SDL_SCANCODE_S])
		rot_y -= rot_speed * delta;
	if (key_state[SDL_SCANCODE_D])
		rot_z -= rot_speed * delta;
	if (key_state[SDL_SCANCODE_R])
		mov_z += mov_speed * delta;
	if (key_state[SDL_SCANCODE_F])
		mov_z -= mov_speed * delta;
	
	rot_x = wrapf(rot_x, -PI, PI);
	rot_y = wrapf(rot_y, -PI, PI);
	rot_z = wrapf(rot_z, -PI, PI);
	
	Matrix4f mz = mat4f_rot_z(rot_z);
	Matrix4f mx = mat4f_rot_x(rot_x);
	Matrix4f my = mat4f_rot_y(rot_y);
	
	model->matrix = mat4f_mul(model->matrix, mz);
	model->matrix = mat4f_mul(model->matrix, mx);
	model->matrix = mat4f_mul(model->matrix, my);
	
	model->matrix = mat4f_trans(model->matrix, 0.0f, 0.0f, mov_z);
}

void handle_input() {
	SDL_Event sdl_event;
	key_state = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&sdl_event))
		if (sdl_event.type == SDL_QUIT)
				running = 0;
}

void sdl_init() {
	sdl_window = SDL_CreateWindow("C Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ctx->w, ctx->h, SDL_WINDOW_SHOWN);
	sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);
	sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, ctx->w, ctx->h);
}

void sdl_render() {
	SDL_UpdateTexture(sdl_texture, NULL, ctx->color_buffer, ctx->w * sizeof(uint32_t));
	SDL_RenderClear(sdl_renderer);
	SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
	SDL_RenderPresent(sdl_renderer);
}

int main() {
	init();
	sdl_init();
	
	clock_t time_previous = clock();
	while (running) {
		clock_t time_current = clock();
		double time_elapsed = difftime(time_current, time_previous) / CLOCKS_PER_SEC;
		printf("FPS: %f\n", 1 / time_elapsed);

		handle_input();
		update(time_elapsed);
		scn_render(ctx, scn);
		sdl_render();

		time_previous = time_current;
	}

	scn_free(scn);
	ctx_free(ctx);

	return 0;
}
