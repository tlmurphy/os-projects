/**
 * Exercise 4.21
 */

#include "pid_manager.h"
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int avg = 0;
int min = 0;
int max = 0;

void *find_avg(void *param) {
  char const **nums = (char const **)param;
  int pid = allocate_pid();
  int count = 0;
  int sum = 0;
  for (size_t i = 0; nums[i]; i++) {
    count++;
    sum += atoi(nums[i]);
  }
  avg = sum / count;
  release_pid(pid);
  pthread_exit(0);
}

void *find_min(void *param) {
  char const **nums = (char const **)param;
  int pid = allocate_pid();
  min = atoi(nums[0]);
  for (size_t i = 0; nums[i]; i++) {
    if (atoi(nums[i]) < min) {
      min = atoi(nums[i]);
    }
  }
  release_pid(pid);
  pthread_exit(0);
}

void *find_max(void *param) {
  char const **nums = (char const **)param;
  int pid = allocate_pid();
  max = atoi(nums[0]);
  for (size_t i = 0; nums[i]; i++) {
    if (atoi(nums[i]) > max) {
      max = atoi(nums[i]);
    }
  }
  release_pid(pid);
  pthread_exit(0);
}

bool isNumber(char const number[]) {
  int i = 0;
  if (number[0] == '-')
    i = 1;
  for (; number[i] != 0; i++) {
    if (!isdigit(number[i]))
      return false;
  }
  return true;
}

int main(int argc, char const *argv[]) {
  char const *nums[argc];

  for (size_t i = 1; i < argc; i++) {
    if (isNumber(argv[i])) {
      nums[i - 1] = argv[i];
    } else {
      puts("You provided a non-integer argument! Boo! Exiting...");
      return -1;
    }
  }
  nums[argc] = NULL;

  srand(time(NULL));
  pthread_t avg_thread;
  pthread_t min_thread;
  pthread_t max_thread;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  allocate_map();

  pthread_create(&avg_thread, &attr, find_avg, (void *)nums);
  pthread_create(&min_thread, &attr, find_min, (void *)nums);
  pthread_create(&max_thread, &attr, find_max, (void *)nums);
  pthread_join(avg_thread, NULL);
  pthread_join(min_thread, NULL);
  pthread_join(max_thread, NULL);

  printf("The average value is %d\n", avg);
  printf("The minimum value is %d\n", min);
  printf("The maximum value is %d\n", max);

  return 0;
}
