#include <stdio.h>
#include "rendering.h"

#define WIDTH 800
#define HEIGHT 600

#define MAX_COLORS 4
#define RGB 3

#define MAX_LINES 24
#define MAX_ELEMENTS (MAX_LINES * WIDTH)

#define SHOW_FPS 1

uint8_t colors[MAX_COLORS][RGB] = {
    {0x8D, 0x6B, 0x94},
    {0xFF, 0xA6, 0x9E},
    {0xFA, 0xF3, 0xDD},
    {0x78, 0xCD, 0xD7}
};

uint8_t bg_colors[3] = {
    0x05, 0x09, 0x0B
};

typedef struct {
  struct {
    int x;
    int y;
    int z;
  };
  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
} point_t;

void shuffle(point_t **array) {
  point_t *temp;
  for (size_t i = MAX_ELEMENTS - 1; i > 0; i--) {
    size_t j = (size_t) random() % i;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void bubblesort(point_t **array) {
  point_t *temp;
  for (size_t i = 0; i < MAX_ELEMENTS - 1; i++) {
    if (array[i]->z > array[i + 1]->z) {
      temp = array[i];
      array[i] = array[i + 1];
      array[i + 1] = temp;
    }
  }
}

int main() {

  const double aspect_ratio = (double) WIDTH / (double) HEIGHT;

  srandom((uint32_t) time(NULL));
  sdl_rendering_t *sdl = init_sdl(WIDTH, HEIGHT, "Visual sort");

  point_t **pts = malloc(sizeof(point_t) * MAX_ELEMENTS);

  int x;
  int dy;
  for (int i = 0; i < MAX_ELEMENTS; i++) {
    pts[i] = malloc(sizeof(point_t));
    x = i / MAX_LINES;
    pts[i]->x = x;
    if (random() % 2 > 0) {
      dy = - i % MAX_LINES;
    } else {
      dy = i % MAX_LINES;
    }
    pts[i]->y = (int) (x / aspect_ratio) + dy;
    pts[i]->z = i;
    uint8_t *color = colors[random() % MAX_COLORS];
    pts[i]->r = color[0];
    pts[i]->g = color[1];
    pts[i]->b = color[2];
  }

  int is_running = 1;

#if SHOW_FPS == 1
  uint32_t start_time;
  uint32_t delta_time;
#endif

  while (is_running) {

#if SHOW_FPS == 1
    start_time = SDL_GetTicks();
#endif

    bubblesort(pts);

    // BG
    SDL_SetRenderDrawColor(sdl->renderer, bg_colors[0], bg_colors[1], bg_colors[2], SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl->renderer);

    // Points
    for (int i = 0; i < MAX_ELEMENTS; i++) {
      SDL_SetRenderDrawColor(sdl->renderer, pts[i]->r, pts[i]->g, pts[i]->b, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawPoint(sdl->renderer, i / MAX_LINES, pts[i]->y);
    }
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
            case SDLK_SPACE:
              shuffle(pts);
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
    }
  }

  shutdown_sdl(sdl);
  return 0;
}