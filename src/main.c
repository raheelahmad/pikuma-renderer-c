#include "display.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <_types/_uint32_t.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/// Decleare an array of vectors/points
const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
vec3_t camera_position = {0, 0, -5};
float fov_factor = 640;

void setup() {
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

  // create an SDL texture that is used to display the color buffer
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  // start loading array of points in the -1/1 cube of size 9 x 9 x 9
  int point_index = 0;
  // 9 points, so 8 spans, divided by 2 (-1 → 1): 0.25
  float increment = (1.0 - -1.0) / (9 - 1);
  for (float x = -1; x <= 1; x += increment) {
    for (float y = -1; y <= 1; y += increment) {
      for (float z = -1; z <= 1; z += increment) {
        vec3_t new_point = {.x = x, .y = y, .z = z};
        cube_points[point_index] = new_point;
        point_index += 1;
      }
    }
  }
}

// Simply project a 3d point on to 2D
vec2_t project(vec3_t point) {
  vec2_t p = {.x = (fov_factor * point.x) / point.z,
              .y = (fov_factor * point.y) / point.z};
  return p;
}

vec3_t rotate(vec3_t point, vec3_t rotation) {
  vec3_t rotated_point = point;
  rotated_point = rotate_x(rotated_point, rotation.x);
  rotated_point = rotate_y(rotated_point, rotation.y);
  rotated_point = rotate_z(rotated_point, rotation.z);
  return rotated_point;
}

vec3_t cube_rotation = {0, 0, 0};

void update() {
  cube_rotation.z += 0.005;
  cube_rotation.y += 0.005;
  cube_rotation.x += 0.005;
  // Project the points on to the projection plane.
  for (int i = 0; i < N_POINTS; i++) {
    // transform
    vec3_t point = cube_points[i];
    point = rotate(point, cube_rotation);

    // move the points away from the camera:
    point.z -= camera_position.z;

    // project
    vec2_t projected_point = project(point);
    projected_points[i] = projected_point;
  }
}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 210, 220, 232, 255);
  SDL_RenderClear(renderer);

  for (int i = 0; i < N_POINTS; i += 1) {
    vec2_t p = projected_points[i];
    draw_rect(p.x + window_width / 2, p.y + window_height / 2, 4, 4, 0x8AAABB);
  }
  render_color_buffer();

  uint32_t color = 0xFF0000;
  clear_color_buffer(color);

  SDL_RenderPresent(renderer);
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      is_running = false;
    }

  default:
    break;
  }
}

int main(void) {
  is_running = initialize_window();
  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();

  return 0;
}
