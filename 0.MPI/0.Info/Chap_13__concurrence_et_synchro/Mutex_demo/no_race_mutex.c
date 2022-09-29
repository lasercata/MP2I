#include <stdio.h>
#include <pthread.h>

int n;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
  pthread_t p1, p2;
  int r1 = pthread_create(&p1, NULL, loop, NULL);
  int r2 = pthread_create(&p2, NULL, loop, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("%d\n", n);
  pthread_mutex_destroy(&lock);
  return r1 && r2;
}
