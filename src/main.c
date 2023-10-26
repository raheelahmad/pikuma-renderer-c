#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "display.h"
#include "vector.h"

/// Decleare an array of vectors/points
const int N_POINTS = 9 * 9 * 9;  
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
float fov_factor = 120;

void setup() {
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * window_width * window_height);

  // create an SDL texture that is used to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  // start loading array of points in the -1/1 cube of size 9 x 9 x 9
  int point_index = 0;
  for (float x = -1; x <= 1; x+=0.25) {
    for (float y = -1; y <= 1; y+=0.25) {
      for (float z = -1; z <= 1; z+=0.25) {
        vec3_t new_point = { .x = x, .y = y, .z = z};
        cube_points[point_index] = new_point;
        point_index += 1;
      }
    }
  }
}

// Simply project a 3d point on to 2D
vec2_t project(vec3_t point) {
  vec2_t p = {
    .x = fov_factor * point.x,
    .y = fov_factor * point.y
  };
  return p;
}

void update() {
  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = cube_points[i];
    vec2_t projected_point = project(point);
    projected_points[i] = projected_point;
  }
}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 210, 220, 232, 255);
  SDL_RenderClear(renderer);

  for (int i = 0; i < N_POINTS; i += 1) {
    vec2_t p = projected_points[i];
    draw_rect(
              p.x + window_width / 2,
              p.y + window_height / 2,
              4, 4, 0x8AAABB
              );
  }
  render_color_buffer();

  SDL_RenderPresent(renderer);
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type)
  {
  case SDL_QUIT:
    is_running = false;
    break;
    case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
    {
      is_running = false;
    }
    
  default:
    break;
  }
}

int main(void)
{
  is_running = initialize_window();
  setup();

  vec3_t my_vec = { 2.0, 1.0, 3.0 };
  size_t size = sizeof(my_vec);
  
  while (is_running)
  {
    process_input();
    update();
    render();
  }

  destroy_window();
  
  return 0;
}
