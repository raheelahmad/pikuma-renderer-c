#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "display.h"

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
}

void update() {

}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 210, 220, 232, 255);
  SDL_RenderClear(renderer);

  // render the color buffer and clear it
  /* clear_color_buffer(0xFFFFFF00); */
  render_grid();
  draw_rect(120, 230, 300, 500, 0x3239AA);
  draw_rect(820, 30, 30, 500, 0x32390A);
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
  
  while (is_running)
  {
    process_input();
    update();
    render();
  }

  destroy_window();
  
  return 0;
}
