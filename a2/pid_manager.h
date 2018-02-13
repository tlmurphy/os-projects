#ifndef PID_MANAGER
#define PID_MANAGER

#define MIN_PID 300
#define MAX_PID 5000

int bit_map[MAX_PID];

int allocate_map(void);
int allocate_pid(void);
void release_pid(int);

#endif /* end of include guard: PID_MANAGER */
