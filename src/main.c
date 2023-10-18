#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool is_running = false;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 800;
int window_height = 800;

bool initialize_window(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // use SDL to query what is the full screen width & height
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode); // populates the mode struct
  window_width = display_mode.w;
  window_height = display_mode.h;


  // create a window
  window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);
  if (!window)
  {
    fprintf(stderr, "Error creating the SDL window");
    return false;
  }
  // create an SDL renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    fprintf(stderr, "Error creating the SDL renderer");
    return false;
  }

  // change video mode to real full screen
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  return true;
}

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

void render_color_buffer(void) {
  // copy the color_buffer to the color_buffer_texture
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (window_width * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  for(int j = 0; j < window_height; j++) {
    for(int i = 0; i < window_width; i++) {
      int index = j * window_width + i;
      color_buffer[index] = color;
    }
  }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for(int j = 0; j < (0 + height); j++) {
    for(int i = 0; i < (0 + width); i++) {
      int index = (y + j) * window_width + (x + i) ;
      color_buffer[index] = color;
    }
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

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
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
