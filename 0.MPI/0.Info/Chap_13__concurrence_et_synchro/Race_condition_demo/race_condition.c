// gcc -lpthread race_condition.c -o prog
#include <stdio.h>
#include <pthread.h>

int n;

void* loop () {
  for (int i = 0; i < 100; i++) {
    int m = n;
    sched_yield ();
    n = m + 1;
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
  return r1 && r2;
}
