
#ifndef VISUALSORT_THREADS_H
#define VISUALSORT_THREADS_H

#include <pthread.h>
#include "sorting.h"

typedef struct {

  point_t **pts;
  size_t pts_count;

  int is_running;
  int should_terminate;

  pthread_mutex_t mutex;
  pthread_cond_t cond;

} pthread_params_t;

typedef struct {
  pthread_t pthread;
  pthread_params_t *params;
} pthread_data_t;

pthread_data_t *init_thread(point_t **pts, size_t pts_count, void *task());

void destroy_thread_data(pthread_data_t *data);

void toggle_is_running(pthread_params_t *params);

void toggle_should_terminate(pthread_params_t *params);

void await_is_running(pthread_params_t *params);

void terminate_if_necessary(pthread_params_t *params);

#endif //VISUALSORT_THREADS_H
