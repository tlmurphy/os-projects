/**
 * Exercise 5.36
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MIN_PID 300
#define MAX_PID 5000
#define NUM_THREADS 100

int counter = 0;
int bit_map[MAX_PID];
pthread_mutex_t mutex;

int allocate_map(void) {
  for (size_t i = MIN_PID; i < MAX_PID; i++) {
    bit_map[i] = 0;
  }
  return 1;
}

int allocate_pid(void) {
  for (size_t i = MIN_PID; i < MAX_PID; i++) {
    if (bit_map[i] == 0) {
      bit_map[i] = 1;
      return i;
    }
  }
  return -1;
}

void release_pid(int pid) {
  bit_map[pid] = 0;
}

void *runner(void *param) {
  pthread_mutex_lock(&mutex);
  counter++;
  int pid = allocate_pid();
  int sleep_time = (rand() % 20) + 1;
  printf("Sleeping Time: %d; Thread Id = %ld; Counter Value = %d\n", sleep_time, (long) pthread_self(), counter);
  pthread_mutex_unlock(&mutex);
  sleep(sleep_time); // Random number from 1 -> 20
  release_pid(pid);
  pthread_exit(0);
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  pthread_t workers[NUM_THREADS];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_mutex_init(&mutex, NULL);
  allocate_map();
  int i = 0;
  while (i < NUM_THREADS) {
    pthread_create(&workers[i], &attr, runner, NULL);
    i++;
  }
  i = 0;
  while (i < NUM_THREADS) {
    pthread_join(workers[i], NULL);
    i++;
  }
  return 0;
}
