#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

int turn = 0;
bool want[2] = {false, false};
int n = 0;

void* thread_fun (void* args) {
  int thread = *((int*) args);
  int other_thread = 1 - thread;
  // pour simuler l'interruption d'un thread avant l'autre
  int bound = thread == 0 ? 50 : 100;
  for (int i = 0; i < bound; i++) {
    want[thread] = true;
    turn = other_thread;
    sched_yield(); // pour simuler une arrivée simultanée
    // attente active
    while (want[other_thread] && turn == other_thread) ;
    // section critique
    n++;
    // sortie de section critique
    want[thread] = false;
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
