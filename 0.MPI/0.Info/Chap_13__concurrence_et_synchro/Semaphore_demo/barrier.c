#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NB_THREADS 5

sem_t barrier;
int waiting;
pthread_mutex_t m;

void* wait_barrier (void* args) {
  int i = *((int*) args);
  pthread_mutex_lock(&m);
  printf("Thread %d reaching barrier\n", i);
  waiting++;
  if (waiting != NB_THREADS) {
    pthread_mutex_unlock(&m);
    sem_wait(&barrier);
  } else {
    for (int j = 0; j < NB_THREADS - 1; j++)
      sem_post(&barrier);
    waiting = 0;
    pthread_mutex_unlock(&m);
  }
  printf("Thread %d leaving barrier\n", i);
}

int main () {
  sem_init(&barrier, 0, 0);
  pthread_mutex_init(&m, NULL);
  pthread_t t[NB_THREADS];
  int t_id[NB_THREADS];
  for (int i = 0; i < NB_THREADS; i++) {
    t_id[i] = i;
    pthread_create(t + i, NULL, wait_barrier, t_id + i);
  }
  for (int i = 0; i < NB_THREADS; i++)
    pthread_join(t[i], NULL);
  pthread_mutex_destroy(&m);
  sem_destroy(&barrier);
  return 0;
}
