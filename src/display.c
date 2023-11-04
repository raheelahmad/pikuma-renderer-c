#include "display.h"
#include <stdbool.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool is_running = false;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

bool fullscreen = true;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // use SDL to query what is the full screen width & height
  if (fullscreen) {
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode); // populates the mode struct
    window_width = display_mode.w;
    window_height = display_mode.h;
  } else {
    window_width = 900;
    window_height = 600;
  }

  // create a window
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating the SDL window");
    return false;
  }
  // create an SDL renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating the SDL renderer");
    return false;
  }

  if (fullscreen) {
    // change video mode to real full screen
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  }

  return true;
}

void render_color_buffer(void) {
  // copy the color_buffer to the color_buffer_texture
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (window_width * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  for (int j = 0; j < window_height; j++) {
    for (int i = 0; i < window_width; i++) {
      int index = j * window_width + i;
      color_buffer[index] = color;
    }
  }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int i = 0; i < (0 + width); i++) {
    for (int j = 0; j < (0 + height); j++) {
      int current_x = (x + i);
      int current_y = (y + j);
      draw_pixel(current_x, current_y, color);
    }
  }
}

// DDA algorithm
void draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
  int delta_x = x2 - x1;
  int delta_y = y2 - y1;
  int longest_side_length =
      abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

  // according to the algorithm, in every step we move the same amount
  float step_x = delta_x / (float)longest_side_length;
  float step_y = delta_y / (float)longest_side_length;

  float current_x = x1;
  float current_y = y1;
  for (int i = 0; i <= longest_side_length; i++) {
    draw_pixel(round(current_x), round(current_y), color);
    current_x += step_x;
    current_y += step_y;
  }
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3,
                   uint32_t color) {
  draw_line(x1, y1, x2, y2, color);
  draw_line(x3, y3, x2, y2, color);
  draw_line(x1, y1, x3, y3, color);
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
    color_buffer[y * window_width + x] = color;
  }
}

void render_grid(void) {
  int num_grids_w = 20;
  int num_grids_h = 20;
  int grid_w = window_width / num_grids_w;
  int grid_h = window_height / num_grids_h;

  int grid_thickness = 1;
  uint32_t color = 0xFFFFFFFF;

  // columns
  for (int j = 0; j < window_height; j++) {
    int grid_h_remainder = (j % grid_h);
    int grid_h_index = j == 0 ? 0 : j / grid_h;

    if (grid_h_index > 0 && grid_h_remainder <= grid_thickness) {
      for (int i = 0; i < window_width; i++) {
        int index = j * window_width + i;
        color_buffer[index] = color;
      }
    } else {

      for (int g_i = 1; g_i < num_grids_w; g_i++) {

        int g_i_start = g_i * grid_w;

        for (int m = 0; m < grid_thickness; m++) {
          int index = j * window_width + g_i_start + m;
          color_buffer[index] = color;
        }
      }
    }
  }
}

void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
