/**
 * Project 4
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void readInput(char *fileName) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("sampleInput.txt", "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    char *token;
    token = strtok(line, ",");
    while (token != NULL) {
        int intInput = atoi(token);
        printf("%d\n", intInput);
        token = strtok (NULL, ",");
    }
  }

  fclose(fp);
  if (line)
    free(line);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Please provide the input file as the argument!");
    exit(EXIT_FAILURE);
  }
  readInput(argv[1]);
  exit(EXIT_SUCCESS);
}
