#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t array = PTHREAD_MUTEX_INITIALIZER;
bool stick[5] = {true, true, true, true, true};

void* loop (void* args) {
  int i = *((int*) args);
  printf("Philosopher %d is starting\n", i);
  for (int j = 0; j < 10; j++) {
    printf("Philosopher %d is thinking\n", i);
    while (true) {
      pthread_mutex_lock(&array);
      if (stick[i] && stick[(i + 1) % 5]) {
        stick[i] = false;
        stick[(i + 1) % 5] = false;
        pthread_mutex_unlock(&array);
        break;
      }
      pthread_mutex_unlock(&array);
    }
    printf("Philosopher %d is eating\n", i);
    stick[i] = true;
    stick[(i + 1) % 5] = true;
  }
  printf("Philosopher %d finished\n", i);
}

int main () {
  pthread_t t[5];
  int t_id[5] = {0, 1, 2, 3, 4};
  for (int i = 0; i < 5; i++)
    pthread_create(t + i, NULL, loop, (void*) (t_id + i));
  for (int i = 0; i < 5; i++)
    pthread_join(t[i], NULL);
  pthread_mutex_destroy(&array);
  return 0;
}
