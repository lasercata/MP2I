#include <stdio.h>
#include <pthread.h>

int n;
pthread_mutex_t lock;

void* loop () {
  for (int i = 0; i < 100; i++) {
    pthread_mutex_lock(&lock);
    int m = n;
    sched_yield ();
    n = m + 1;
    pthread_mutex_unlock(&lock);
  }
}

int main () {
  n = 0;
  pthread_mutex_init(&lock, NULL);
  pthread_t t[10];
  for (int i = 0; i < 10; i++)
    pthread_create(t + i, NULL, loop, NULL);
  for (int i = 0; i < 10; i++)
    pthread_join(t[i], NULL);
  printf("%d\n", n);
  pthread_mutex_destroy(&lock);
  return 0;
}
