#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

pthread_mutex_t stick[5];
sem_t pick_up;

void* loop (void* args) {
  int i = *((int*) args);
  printf("Philosopher %d is starting\n", i);
  for (int j = 0; j < 10; j++) {
    printf("Philosopher %d is thinking\n", i);
    sem_wait(&pick_up);
    pthread_mutex_lock(stick + i);
    sched_yield();
    pthread_mutex_lock(stick + ((i + 1) % 5));
    sem_post(&pick_up);
    printf("Philosopher %d is eating\n", i);
    pthread_mutex_unlock(stick + i);
    pthread_mutex_unlock(stick + ((i + 1) % 5));
  }
  printf("Philosopher %d finished\n", i);
}

int main () {
  for (int i = 0; i < 5; i++)
    pthread_mutex_init(stick + i, NULL);
  sem_init(&pick_up, 0, 4);
  pthread_t t[5];
  int t_id[5] = {0, 1, 2, 3, 4};
  for (int i = 0; i < 5; i++)
    pthread_create(t + i, NULL, loop, (void*) (t_id + i));
  for (int i = 0; i < 5; i++)
    pthread_join(t[i], NULL);
  for (int i = 0; i < 5; i++)
    pthread_mutex_destroy(stick + i);
  sem_destroy(&pick_up);
  return 0;
}
