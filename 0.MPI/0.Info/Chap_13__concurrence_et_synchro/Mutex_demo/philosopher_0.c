#include <stdio.h>
#include <pthread.h>

pthread_mutex_t diner;

void* loop (void* args) {
  int i = *((int*) args);
  printf("Philosopher %d is starting\n", i);
  for (int j = 0; j < 10; j++) {
    printf("Philosopher %d is thinking\n", i);
    pthread_mutex_lock(&diner);
    printf("Philosopher %d is eating\n", i);
    sched_yield ();
    pthread_mutex_unlock(&diner);
  }
  printf("Philosopher %d finished\n", i);
}

int main () {
  pthread_mutex_init(&diner, NULL);
  pthread_t t[5];
  int t_id[5] = {0, 1, 2, 3, 4};
  for (int i = 0; i < 5; i++)
    pthread_create(t + i, NULL, loop, (void*) (t_id + i));
  for (int i = 0; i < 5; i++)
    pthread_join(t[i], NULL);
  pthread_mutex_destroy(&diner);
  return 0;
}
