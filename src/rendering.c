#include <stdio.h>
#include <SDL2/SDL.h>
#include "rendering.h"

sdl_rendering_t *init_sdl(sdl_params_t params) {

  sdl_rendering_t *sdl = malloc(sizeof(sdl_rendering_t));

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Failed to init SDL: %s", SDL_GetError());
    return NULL;
  }

  sdl->window = SDL_CreateWindow(params.title, 0, 0, params.w, params.h, SDL_WINDOW_SHOWN);
  if (sdl->window == NULL) {
    printf("Failed to init SDL Window: %s", SDL_GetError());
    SDL_Quit();
    return NULL;
  }

  sdl->ctx = SDL_GL_CreateContext(sdl->window);
  if (sdl->ctx == NULL) {
    printf("Failed to init SDL GLContext: %s", SDL_GetError());
    SDL_DestroyWindow(sdl->window);
    SDL_Quit();
    return NULL;
  }

#ifdef __APPLE__
  // MacOSX Mojave hack to display window contents immediately
  SDL_PumpEvents();
#endif

  sdl->renderer = SDL_CreateRenderer(sdl->window, -1,
                                     SDL_RENDERER_ACCELERATED);
  if (sdl->renderer == NULL) {
    printf("Failed to init SDL Renderer: %s", SDL_GetError());
    SDL_DestroyWindow(sdl->window);
    SDL_Quit();
    return NULL;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  sdl->event = malloc(sizeof(SDL_Event));
  sdl->max_fps = params.max_fps;
  sdl->max_frame_time = 1000 / params.max_fps;

  printf("SDL is initialized. %dx%d with max fps %d\n", params.w, params.h, sdl->max_fps);
  return sdl;
}

void shutdown_sdl(sdl_rendering_t *sdl) {
  if (sdl != NULL) {
    if (sdl->renderer != NULL) {
      SDL_DestroyRenderer(sdl->renderer);
    }
    if (sdl->window != NULL) {
      SDL_DestroyWindow(sdl->window);
    }
    SDL_Quit();
    free(sdl->ctx);
    free(sdl->event);
    free(sdl);
    printf("SDL is shut down\n");
  }
}

void delay_frame(sdl_rendering_t *sdl) {
  sdl->frame_time = SDL_GetTicks() - sdl->frame_start;
  if (sdl->frame_time < sdl->max_frame_time) {
    SDL_Delay(sdl->max_frame_time - sdl->frame_time);
  }
  sdl->frame_start = SDL_GetTicks();
}
