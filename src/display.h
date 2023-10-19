#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>


SDL_Window *window;
SDL_Renderer *renderer;
bool is_running;

uint32_t *color_buffer;
SDL_Texture *color_buffer_texture;

int window_width;
int window_height;


bool initialize_window(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void render_grid(void);
void destroy_window(void);
