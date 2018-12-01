

#ifndef VISUALSORT_RENDERING_H
#define VISUALSORT_RENDERING_H

#include <SDL2/SDL.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_GLContext *ctx;
  SDL_Event *event;
  uint32_t frame_start;
  uint32_t frame_time;
  int max_fps;
  int max_frame_time;
} sdl_rendering_t;

typedef struct {
  int w;
  int h;
  char *title;
  int max_fps;
} sdl_params_t;

sdl_rendering_t *init_sdl(sdl_params_t params);

void shutdown_sdl(sdl_rendering_t *sdl);

void delay_frame(sdl_rendering_t *sdl);

#endif //VISUALSORT_RENDERING_H
