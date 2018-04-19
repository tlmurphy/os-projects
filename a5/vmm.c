#include <stdio.h>
#include <stdlib.h>

typedef struct pageTable {
  int *pages;
  int size;
  int faults;
} PageTable;

typedef struct tlb {
  int *pages;
  int *frames;
  int size;
  int hits;
} Tlb;

Tlb *tlb;
PageTable *pageTable;
signed char **frames;
int head = 0;

int getFromTlb(int pageNumber);
void insertIntoTlb(int, int);
int getFrameIndex();
int getFromPageTable(int, int);
void translate(int);
void createTlb();
void createPageTable();
void freeMemory();

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Please provide addresses file.\n");
    exit(1);
  }

  frames = calloc(256, sizeof(signed char *));
  createTlb();
  createPageTable();

  FILE *addresses = fopen(argv[1], "r");
  if (addresses == NULL) {
    fprintf(stderr, "Error reading addresses file.\n");
    exit(1);
  }

  int value;
  int total = 0;
  fscanf(addresses, "%d", &value);
  while (!feof(addresses)) {
    translate(value);
    total++;
    fscanf(addresses, "%d", &value);
  }
  fclose(addresses);

  printf("Number of Translated Addresses = %d\n", total);
  printf("Page Faults = %d\n", pageTable->faults);
  printf("Page Fault Rate = %.3f\n", (double)pageTable->faults / total);
  printf("TLB Hits = %d\n", tlb->hits);
  printf("TLB Hit Rate = %.3f\n", (double)tlb->hits / total);

  freeMemory();
  return 0;
}

int getFromTlb(int pageNumber) {
  for (int i = 15; i > -1; i--) {
    if (tlb->pages[i] == pageNumber) {
      tlb->hits++;
      return tlb->frames[i];
    }
  }
  return -1;
}

void insertIntoTlb(int pageNumber, int frameNumber) {
  for (int i = 0; i < 15; i++) {
    tlb->pages[i] = tlb->pages[i + 1];
    tlb->frames[i] = tlb->frames[i + 1];
  }

  tlb->pages[15] = pageNumber;
  tlb->frames[15] = frameNumber;
}

int getFrameIndex() {
  int setIndex = head;
  if (frames[setIndex] != 0)
    free(frames[setIndex]);
  head++;
  if (head == 256)
    head = 0;
  return setIndex;
}

int getFromPageTable(int pageNumber, int offset) {
  if (pageTable->pages[pageNumber] != -1) {
    return pageTable->pages[pageNumber];
  }

  // Else: Page fault occurs
  pageTable->faults++;

  signed char *data = malloc(sizeof(signed char) * 256);
  FILE *backingStore = fopen("BACKING_STORE.bin", "rb");
  if (fseek(backingStore, pageNumber * 256, SEEK_SET) != 0)
    fprintf(stderr, "Error seeking from backing store.\n");
  if (fread(data, sizeof(signed char), 256, backingStore) == 0)
    fprintf(stderr, "Error reading from backing.\n");

  int index = getFrameIndex();
  pageTable->pages[pageNumber] = index;
  frames[index] = data;

  fclose(backingStore);

  return index;
}

void translate(int logicalAddress) {
  int pageNumber = (logicalAddress & 0xFFFF) >> 8;
  int offset = (logicalAddress & 0xFF);

  int frameIndex = getFromTlb(pageNumber);

  if (frameIndex < 0) {
    frameIndex = getFromPageTable(pageNumber, offset);
    insertIntoTlb(pageNumber, frameIndex);
  }

  printf("Virtual address: %d ", logicalAddress);
  printf("Physical address: %d ", (frameIndex << 8) | offset);
  printf("Value: %d\n", frames[frameIndex][offset]);
}

void createTlb() {
  tlb = malloc(sizeof(Tlb));
  tlb->size = 0;
  tlb->hits = 0;
  tlb->pages = calloc(16, sizeof(int));
  tlb->frames = calloc(16, sizeof(int));
  for (int i = 0; i < 16; i++) {
    tlb->frames[i] = -1;
  }
}

void createPageTable() {
  pageTable = malloc(sizeof(pageTable));
  pageTable->size = 0;
  pageTable->faults = 0;
  pageTable->pages = malloc(sizeof(int) * 256);
  for (int i = 0; i < 256; i++) {
    pageTable->pages[i] = -1;
  }
}

void freeMemory() {
  free(tlb->pages);
  free(tlb->frames);
  free(pageTable->pages);
  for (int i = 0; i < 256; i++) {
    if (frames[i] != 0)
      free(frames[i]);
  }
  free(tlb);
  free(frames);
  free(pageTable);
}
