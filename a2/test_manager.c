/**
 * Exercise 4.20
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
// #include <sys/types.h>
// #include <time.h>
#include <unistd.h>
#include "pid_manager.h"

#define NUM_THREADS 100

void *runner(void *param) {
  int pid = allocate_pid();
  printf("Allocated PID %d\n", pid);
  sleep(20);
  release_pid(pid);
  printf("Released PID %d\n", pid);
  pthread_exit(0);
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  pthread_t workers[NUM_THREADS];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  allocate_map();
  for (size_t i = 0; i < NUM_THREADS; i++) {
    pthread_create(&workers[i], &attr, runner, NULL);
  }
  for (size_t i = 0; i < NUM_THREADS; i++) {
    pthread_join(workers[i], NULL);
  }
  return 0;
}
