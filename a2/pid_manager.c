/**
 * Exercise 3.20
 */

#include <stdlib.h>
#include "pid_manager.h"


int allocate_map(void) {
  for (size_t i = MIN_PID; i < MAX_PID; i++) {
    bit_map[i] = 0;
  }
  return 1;
}

int allocate_pid(void) {
  for (size_t i = 300; i < MAX_PID; i++) {
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
