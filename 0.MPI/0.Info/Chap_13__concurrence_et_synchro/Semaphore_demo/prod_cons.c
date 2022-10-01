#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NB_PROD 2
#define NB_CONS 5
#define BUFFER_SIZE 5

typedef struct {
  int array[BUFFER_SIZE];
  int start;
  int end;
} buffer;

void init (buffer* b) {
  b->start = 0;
  b->end = 0;
}

void add (int n, buffer* b) {
  b->array[b->end] = n;
  b->end = (b->end + 1) % BUFFER_SIZE;
}

int take (buffer* b) {
  int n = b->array[b->start];
  b->start = (b->start + 1) % BUFFER_SIZE;
  return n;
}

buffer b;
sem_t empty;
sem_t full;
pthread_mutex_t access;

void* producer (void* args) {
  int i = *((int*) args);
  for (int j = 0; j < NB_CONS; j++) {
    sem_wait(&empty);
    pthread_mutex_lock(&access);
    int k = rand () % 100;
    printf("Producer %d writing %d\n", i, k);
    add(k, &b);
    pthread_mutex_unlock(&access);
    sem_post(&full);
  }
}

void* consumer (void* args) {
  int i = *((int*) args);
  for (int j = 0; j < NB_PROD; j++) {
    sem_wait(&full);
    pthread_mutex_lock(&access);
    int k = take(&b);
    printf("Consumer %d reading %d\n", i, k);
    pthread_mutex_unlock(&access);
    sem_post(&empty);
  }
}

int main () {
  srand(time(NULL));
  init(&b);
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);
  pthread_mutex_init(&access, NULL);
  pthread_t p[NB_PROD], c[NB_CONS];
  int p_id[NB_PROD], c_id[NB_CONS];
  for (int i = 0; i < NB_CONS; i++) {
    c_id[i] = i;
    pthread_create(c + i, NULL, consumer, c_id + i);
  }
  for (int i = 0; i < NB_PROD; i++) {
    p_id[i] = i;
    pthread_create(p + i, NULL, producer, p_id + i);
  }
  for (int i = 0; i < NB_CONS; i++)
    pthread_join(c[i], NULL);
  for (int i = 0; i < NB_PROD; i++)
    pthread_join(p[i], NULL);
  pthread_mutex_destroy(&access);
  sem_destroy(&empty);
  sem_destroy(&full);
  return 0;
}
