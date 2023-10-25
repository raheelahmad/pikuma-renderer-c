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
  for (float x = -1; x <= 1; x+=2.0/9.0) {
    for (float y = -1; y <= 1; y+=2.0/9.0) {
      for (float z = -1; z <= 1; z+=2.0/9.0) {
        vec3_t new_point = { .x = x, .y = y, .z = z};
        cube_points[point_index] = new_point;
        point_index += 1;
      }
    }
  }
}

void update() {

}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 210, 220, 232, 255);
  SDL_RenderClear(renderer);

  // render the color buffer and clear it
  /* clear_color_buffer(0xFFFFFF00); */
  /* render_grid(); */
  /* draw_rect(120, 230, 300, 500, 0x3239AA); */
  /* draw_rect(820, 30, 30, 500, 0x32390A); */
  draw_pixel(220, 30, 0x3239FA);
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
  printf("Size of vector is %zu", size);
  /* size = sizeof(struct vec3_t); */
  /* printf("Size of vector struct is %zu", size); */
  
  while (is_running)
  {
    process_input();
    update();
    render();
  }

  destroy_window();
  
  return 0;
}
