#include "sorting.h"
#include <stdlib.h>
#include <stdio.h>
#include "threads.h"

void swap(point_t **pts, size_t i, size_t j) {
//  printf("swap %zu %zu\n", i, j);
  point_t *temp;
  temp = pts[i];
  pts[i] = pts[j];
  pts[j] = temp;
}

void *run_bubble_sort(void *arg) {
  printf("Spawned bubble sort thread\n");
  pthread_params_t *params = (pthread_params_t *) arg;
  size_t j;
  int swaps_count;
  do {
    swaps_count = 0;
    await_is_running(params);
    for (size_t i = 0; i < params->pts_count - 1; i++) {
      terminate_if_necessary(params);
      j = i + 1;
      if (params->pts[i]->x > params->pts[j]->x) {
        swaps_count += 1;
        swap(params->pts, i, j);
      }
    }
  } while (swaps_count != 0);
  // Exit thread
  printf("Finished sorting\n");
  return NULL;
}

void *run_selection_sort(void *arg) {
  printf("Spawned selection sort thread\n");
  pthread_params_t *params = (pthread_params_t *) arg;
  for (size_t i = 0; i < params->pts_count - 1; i++) {
    for (size_t j = i + 1; j < params->pts_count; j++) {
      await_is_running(params);
      terminate_if_necessary(params);
      printf("Now running...\n");
      if (params->pts[i] > params->pts[j]) {
        swap(params->pts, i, j);
      }
    }
  }
  // Exit thread
  printf("Finished sorting\n");
  return NULL;
}

void shuffle(point_t **pts, size_t pts_count) {
  point_t *temp;
  srandom((uint32_t) time(NULL));
  for (size_t i = pts_count - 1; i > 0; i--) {
    size_t j = (size_t) random() % i;
    temp = pts[i];
    pts[i] = pts[j];
    pts[j] = temp;
  }
}
