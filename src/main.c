#include "display.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_null.h>

bool drawing_sierpinski = false;

uint64_t previous_frame_time;

/// Declare an array of vectors/points
const int CUBE_POINTS = 9 * 9 * 9;
/// The model's points in 3D space
vec3_t cube_points[CUBE_POINTS];
/// The projected model points on to the 2D space. These are scaled to be in the
/// screen space.
vec2_t projected_points[CUBE_POINTS];

const int SIERPINSKI_MAX_COUNT = 10000;
int current_sierpinski_count = 3;
vec3_t sierpinski_points[SIERPINSKI_MAX_COUNT];
vec2_t sierpinski_projected_points[SIERPINSKI_MAX_COUNT];

vec3_t camera_position = {0, 0, -5};
float fov_factor = 1240;

void setup_cube_points() {
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

void setup_sierpinski_points() {
  // top
  vec3_t p0 = {0, -0.5, 0};
  // bottom left
  vec3_t p1 = {-0.5, 0.5, 0};
  // bottom right
  vec3_t p2 = {0.5, 0.5, 0};
  sierpinski_points[0] = p0;
  sierpinski_points[1] = p1;
  sierpinski_points[2] = p2;
}

void setup() {
  srand(2011);
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

  // create an SDL texture that is used to display the color buffer
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  if (drawing_sierpinski) {
    setup_sierpinski_points();
  } else {
    setup_cube_points();
  }
}

// Simply project a 3d point on to 2D
vec2_t project(vec3_t point) {
  vec2_t p = {.x = (fov_factor * point.x) / point.z,
              .y = (fov_factor * point.y) / point.z};
  return p;
}

vec3_t cube_rotation = {0, 0, 0};

void update_cube() {
  cube_rotation.z += 0.005;
  cube_rotation.y += 0.005;
  cube_rotation.x += 0.005;
  // Project the points on to the projection plane.
  for (int i = 0; i < CUBE_POINTS; i++) {
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

// ------ SIERPINSKI -------
vec3_t initial_sierpinski_point() {
  vec3_t p = {0.1, 0.2, 0};
  return p;
}

void add_sierpinski_point() {
  if (current_sierpinski_count >= SIERPINSKI_MAX_COUNT) {
    return;
  }

  int next_index = current_sierpinski_count + 1;
  vec3_t next_point;
  if (current_sierpinski_count == 3) {
    next_point = initial_sierpinski_point();
  } else {
    // pick the last point we added:
    int p_index = current_sierpinski_count - 1;
    vec3_t p = sierpinski_points[p_index];

    // and one of the three vertex:
    int vertex_index = rand() % (3);
    vec3_t vertex = sierpinski_points[vertex_index];

    // next point is at the mid point:
    vec3_t q = {
        .x = (p.x + vertex.x) / 2,
        .y = (p.y + vertex.y) / 2,
        .z = (p.z + vertex.z) / 2,
    };
    next_point = q;
  }

  sierpinski_points[next_index] = next_point;
  current_sierpinski_count += 1;
}

void update_sierpinski() {
  add_sierpinski_point();

  cube_rotation.z += 0.005;
  cube_rotation.y += 0.005;
  cube_rotation.x += 0.005;

  for (int i = 0; i < current_sierpinski_count; i++) {
    vec3_t point = sierpinski_points[i];
    point = rotate(point, cube_rotation);
    point.z -= camera_position.z;
    vec2_t projected_point = project(point);
    sierpinski_projected_points[i] = projected_point;
  }
}
// -------------------------

void update() {
  // Respect the FPS;
  // don't update the view until we reach the target time based on the FPS
  while (!SDL_TICKS_PASSED(SDL_GetTicks64(), previous_frame_time + FRAME_TARGET_TIME));
  previous_frame_time = SDL_GetTicks64();
  
  if (drawing_sierpinski) {
    update_sierpinski();
  } else {
    update_cube();
  }
}

void draw_cube() {
  for (int i = 0; i < CUBE_POINTS; i += 1) {
    vec2_t p = projected_points[i];
    draw_rect(p.x + window_width / 2, p.y + window_height / 2, 4, 4, 0x8AAABB);
  }
}

void draw_sierpinski() {
  for (int i = 0; i < current_sierpinski_count; i++) {
    vec2_t p = sierpinski_projected_points[i];
    draw_rect(p.x + window_width / 2, p.y + window_height / 2, 2, 2, 0x8AAABB);
  }
}

void render(void) {
  // Set background color; but we are always rendering the full screen with our
  // buffer, so don't need this. SDL_SetRenderDrawColor(renderer, 210, 220, 232,
  // 255); SDL_RenderClear(renderer);

  // clear our buffer memory so we have a blank slate
  uint32_t color = 0xFF0000;
  clear_color_buffer(color);

  // draw on to the buffer
  if (drawing_sierpinski) {
    draw_sierpinski();
  } else {
    draw_cube();
  }

  // render
  render_color_buffer();

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
