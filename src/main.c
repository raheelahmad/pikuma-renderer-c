#include "array.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
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

uint64_t previous_frame_time;

/// The projected triangles to render
triangle_t *projected_triangles_to_render;

vec3_t camera_position = {0, 0, -6};
float fov_factor = 1240;

void setup() {
  srand(2011);
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

  // create an SDL texture that is used to display the color buffer
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
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

  // initialize the array of triangles to render
  // we reset for each frame:
  projected_triangles_to_render = NULL;

  // Project the points on to the projection plane.
  // loop all the faces of the cube mesh
  for (int f = 0; f < N_MESH_FACES; f++) {
    // each face has 3 indices (face.a, .b, .c) for vertices for a triangle
    face_t mesh_face = mesh_faces[f];
    vec3_t face_vertices[3];
    // mesh face's a,b,c are 1-based index into the vertices held
    // in `mesh_vertices`.
    // So here we are collecting all the 3-D vertices that make up
    // the current mesh_face.
    face_vertices[0] = mesh_vertices[mesh_face.a - 1];
    face_vertices[1] = mesh_vertices[mesh_face.b - 1];
    face_vertices[2] = mesh_vertices[mesh_face.c - 1];

    // set of 3 projected 2-D vertices from the face's 3-D vertices.
    triangle_t projected_triangle;
    // project and store each of the 3 vertices:
    for (int i = 0; i < 3; i++) {
      vec3_t point = face_vertices[i];

      // apply this random transformation
      point = rotate(point, cube_rotation);

      // move the points away from the camera:
      point.z -= camera_position.z;

      // project the current vertex:
      vec2_t projected_point = project(point);

      // scale and translate the projected point to the middle of the screen:
      projected_point.x += window_width / 2;
      projected_point.y += window_height / 2;
      projected_triangle.points[i] = projected_point;
    }

    // save the projected triangle in the array of triangles to render:
    array_push(projected_triangles_to_render, projected_triangle);
  }
}

void update() {
  // Respect the FPS;
  // don't update the view until we reach the target time based on the FPS
  int time_to_wait =
      FRAME_TARGET_TIME - (SDL_GetTicks64() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }
  previous_frame_time = SDL_GetTicks64();

  update_cube();
}

void draw_cube() {
  // get the number of triangles
  int faces_count = array_length(projected_triangles_to_render);

  for (int f = 0; f < faces_count; f += 1) {
    triangle_t triangle = projected_triangles_to_render[f];
    draw_triangle(triangle.points[0].x, triangle.points[0].y,
                  triangle.points[1].x, triangle.points[1].y,
                  triangle.points[2].x, triangle.points[2].y,
                  0x8AAA09 + 20 * f);
    draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0x8AAABB);
    draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0x8AAABB);
    draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0x8AAABB);
  }
}

void render(void) {
  // Set background color; but we are always rendering the full screen with our
  // buffer, so don't need this. SDL_SetRenderDrawColor(renderer, 210, 220, 232,
  // 255); SDL_RenderClear(renderer);

  // clear our buffer memory so we have a blank slate
  uint32_t color = 0xFF0000;
  clear_color_buffer(color);

  draw_cube();

  array_free(projected_triangles_to_render);

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
