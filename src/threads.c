

#include <stdio.h>
#include <stdlib.h>
#include "threads.h"

pthread_data_t *init_thread(point_t **pts, size_t pts_count, void *task()) {

  pthread_data_t *pthread_data = malloc(sizeof(pthread_data_t));
  pthread_params_t *pthread_params = malloc(sizeof(pthread_data_t));

  pthread_mutex_init(&(pthread_params->mutex), NULL);
  pthread_cond_init(&(pthread_params->cond), NULL);

  pthread_params->pts = pts;
  pthread_params->is_running = 1;
  pthread_params->should_terminate = 0;
  pthread_params->pts_count = pts_count;

  pthread_data->params = pthread_params;
  pthread_t pthread;
  pthread_create(&pthread, NULL, task, pthread_params);
  return pthread_data;
}

void destroy_thread_data(pthread_data_t *data) {
  if (data != NULL) {
    free(data->params);
    free(data);
  }
}

void toggle_is_running(pthread_params_t *params) {
  pthread_mutex_lock(&(params->mutex));
  (params->is_running) = !(params->is_running);
  pthread_cond_signal(&(params->cond));
  pthread_mutex_unlock(&(params->mutex));
}

void toggle_should_terminate(pthread_params_t *params) {
  pthread_mutex_lock(&(params->mutex));
  (params->should_terminate) = !(params->should_terminate);
  pthread_cond_signal(&(params->cond));
  pthread_mutex_unlock(&(params->mutex));
}

void await_is_running(pthread_params_t *params) {
  pthread_mutex_lock(&(params->mutex));
  while (params->is_running != 1) {
    pthread_cond_wait(&(params->cond), &(params->mutex));
  }
  pthread_mutex_unlock(&(params->mutex));
}

void terminate_if_necessary(pthread_params_t *params) {
  if ((params->should_terminate) == 1) {
    printf("Terminating...\n");
    pthread_exit(NULL);
  }
}
