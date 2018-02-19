/**
 * Exercise 3.20
 */

#include <stdlib.h>
#include <stdio.h>

#define MIN_PID 300
#define MAX_PID 5000

int bit_map[MAX_PID];

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

int main(int argc, char const *argv[]) {
  allocate_map();
  puts("Allocated Map");
  int pid = allocate_pid();
  printf("Allocated PID %d\n", pid);
  release_pid(pid);
  printf("Released PID %d\n", pid);
  return 0;
}
