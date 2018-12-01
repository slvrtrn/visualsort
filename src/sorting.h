

#ifndef VISUALSORT_POINT_H
#define VISUALSORT_POINT_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {

  struct {
    int x;
    int y;
  };

  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

} point_t;

typedef enum {
  BUBBLE = (1 << 0),
  SELECTION = (1 << 1)
} SORTING_METHOD;

void *run_bubble_sort(void *arg);

void *run_selection_sort(void *arg);

void shuffle(point_t **pts, size_t pts_count);

#endif //VISUALSORT_POINT_H
