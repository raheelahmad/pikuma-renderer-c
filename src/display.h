#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

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
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3,
                   uint32_t color);
void render_grid(void);
void destroy_window(void);
