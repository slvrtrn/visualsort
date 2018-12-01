#include <stdio.h>
#include "rendering.h"
#include "threads.h"

#define TITLE "???"
#define WIDTH 800
#define HEIGHT 600

#define MAX_COLORS 4
#define RGB 3

#define MAX_LINES 48
#define MAX_ELEMENTS (MAX_LINES * WIDTH)

#define SHOW_FPS 0
#define ENABLE_Y_DEVIATION 0

uint8_t colors[MAX_COLORS][RGB] = {
    {0x8D, 0x6B, 0x94},
    {0xFF, 0xA6, 0x9E},
    {0xFA, 0xF3, 0xDD},
    {0x78, 0xCD, 0xD7}
};

uint8_t bg_colors[3] = {
    0x05,
    0x09,
    0x0B
};

void draw_points(sdl_rendering_t *sdl, point_t **pts) {
  for (int i = 0; i < MAX_ELEMENTS; i++) {
    SDL_SetRenderDrawColor(sdl->renderer, pts[i]->r, pts[i]->g, pts[i]->b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(sdl->renderer, i / MAX_LINES, pts[i]->y);
  }
}

int main() {

  const double aspect_ratio = (double) WIDTH / (double) HEIGHT;

  sdl_params_t sdl_params = {WIDTH, HEIGHT, TITLE, 60};
  sdl_rendering_t *sdl = init_sdl(sdl_params);

  point_t **pts = malloc(sizeof(point_t *) * MAX_ELEMENTS);

  int x;
  int dy = 0;
  for (int i = 0; i < MAX_ELEMENTS; i++) {
    pts[i] = malloc(sizeof(point_t));
    x = i / MAX_LINES;
    pts[i]->x = i;
#if ENABLE_Y_DEVIATION == 1
    if (random() % 2 > 0) {
      dy = -i % MAX_LINES;
    } else {
      dy = i % MAX_LINES;
    }
#endif
    pts[i]->y = (int) (x / aspect_ratio) + dy;
    uint8_t *color = colors[random() % MAX_COLORS];
    pts[i]->r = color[0];
    pts[i]->g = color[1];
    pts[i]->b = color[2];
  }

  shuffle(pts, MAX_ELEMENTS);

  int is_running = 1;
  int is_sorting = 0;

  pthread_data_t *pthread_data = NULL;

  SORTING_METHOD sorting_method = BUBBLE;

#if SHOW_FPS == 1
  uint32_t start_time;
  uint32_t delta_time;
#endif

  while (is_running) {

#if SHOW_FPS == 1
    start_time = SDL_GetTicks();
#endif

    // BG
    SDL_SetRenderDrawColor(sdl->renderer, bg_colors[0], bg_colors[1], bg_colors[2], SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl->renderer);

    if (is_sorting == 1) {
      if (sorting_method & BUBBLE) {
        if (pthread_data == NULL) {
          pthread_data = init_thread(pts, MAX_ELEMENTS, run_bubble_sort);
        }
      }
      if (sorting_method & SELECTION) {
        if (pthread_data == NULL) {
          pthread_data = init_thread(pts, MAX_ELEMENTS, run_selection_sort);
        }
      }
    }

    draw_points(sdl, pts);
    SDL_RenderPresent(sdl->renderer);

#if SHOW_FPS == 1
    delta_time = SDL_GetTicks() - start_time;
    if (delta_time > 0) {
      printf("FPS: %d\n", 1000 / delta_time);
    }
#endif

    if (SDL_PollEvent(sdl->event) != 0) {
      switch (sdl->event->type) {
        case SDL_QUIT:
          printf("Received a signal to quit\n");
          is_running = 0;
          break;
        case SDL_KEYDOWN:
          switch (sdl->event->key.keysym.sym) {
            case SDLK_BACKSPACE:
              if (pthread_data != NULL) {
                toggle_should_terminate(pthread_data->params);
                destroy_thread_data(pthread_data);
                pthread_data = NULL;
              }
              is_sorting = 1;
              shuffle(pts, MAX_ELEMENTS);
              break;
            case SDLK_SPACE:
              is_sorting = !is_sorting;
              if (pthread_data != NULL) {
                toggle_is_running(pthread_data->params);
              }
              break;
            case SDLK_1:
              printf("Bubble sort\n");
              sorting_method = BUBBLE;
              is_sorting = 0;
              break;
            case SDLK_2:
              printf("Selection sort\n");
              sorting_method = SELECTION;
              is_sorting = 0;
            default:
              break;
          }
          break;
        default:
          break;
      }
    }

    delay_frame(sdl);
  }

  destroy_thread_data(pthread_data);
  shutdown_sdl(sdl);
  return 0;
}