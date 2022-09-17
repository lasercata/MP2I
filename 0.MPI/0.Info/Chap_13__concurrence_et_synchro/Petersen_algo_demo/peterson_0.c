#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

bool flag[2] = {false, false};
int n = 0;

void* thread_fun (void* args) {
  int thread = *((int*) args);
  int other_thread = 1 - thread;
  for (int i = 0; i < 100; i++) {
    // attente active
    while (flag[other_thread]) ;
    sched_yield(); // pour simuler une interruption
    flag[thread] = true;
    // section critique
    n++;
    // sortie de section critique
    flag[thread] = false;
  }
}

int main () {
  pthread_t p1, p2;
  const int id_1 = 0, id_2 = 1;
  int r1 = pthread_create(&p1, NULL, thread_fun, (void*) &id_1);
  int r2 = pthread_create(&p2, NULL, thread_fun, (void*) &id_2);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("%d\n", n);
  return r1 && r2;
}
