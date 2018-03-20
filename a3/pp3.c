/**
 * Programming Project 3
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 5

typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty, full;
int b_index; // Keep track of buffer index

int insert_item(buffer_item item) {
  if (b_index < BUFFER_SIZE) {
    buffer[b_index] = item;
    b_index++;
    return 0;
  } else {
    return -1;
  }
}

int remove_item(buffer_item *item) {
  if (b_index > 0) {
    *item = buffer[b_index - 1];
    b_index--;
    return 0;
  } else {
    return -1;
  }
}

void *producer(void *param) {
  buffer_item item;
  while (1) {
    sleep((rand() % 10) + 1);
    item = rand();
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    if (insert_item(item)) {
      puts("Error on inserting item, buffer is full!");
    } else {
      printf("Thread %ld producing item %d; Buffer Size: %d\n", (long) pthread_self(), item, b_index);
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
}

void *consumer(void *param) {
  buffer_item item;
  while (1) {
    sleep((rand() % 10) + 1);
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    if (remove_item(&item)) {
      puts("Error on removing item, buffer is empty!");
    } else {
      printf("Thread %ld consuming item %d; Buffer Size: %d\n", (long) pthread_self(), item, b_index);
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
  }
}

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    puts("Please supply 3 arguments!");
    return -1;
  }

  long sleep_time = strtol(argv[1], NULL, 0);
  long n_producers = strtol(argv[2], NULL, 0);
  long n_consumers = strtol(argv[3], NULL, 0);

  srand(time(NULL));
  sem_init(&empty, 0, BUFFER_SIZE); // Empty will count num of empty buffers
  sem_init(&full, 0, 0); // Full will count num of full buffers
  pthread_mutex_init(&mutex, NULL);
  b_index = 0;

  pthread_t prod[n_producers];
  pthread_t cons[n_consumers];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  for (size_t i = 0; i < n_producers; i++) {
    pthread_create(&prod[i], &attr, producer, NULL);
  }
  for (size_t i = 0; i < n_consumers; i++) {
    pthread_create(&cons[i], &attr, consumer, NULL);
  }
  sleep(sleep_time);
  return 0;
}
