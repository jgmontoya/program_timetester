#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
  if(argc < 3)
  {
    printf("Usage: %s <n> <./restore> <arg1> <arg2>. . .\n", argv[0]);
    printf("\tn: How many times should the program be run\n");
    printf("\t./restore: program to test\n");
    printf("\targ1: programs input\n");
    return 1;
  }

  struct timespec start, end;
  int pid, stat;
  double time_used = 0;

  int repeat = strtol(argv[1], NULL, 10);

  char* argv2[argc+1];
  for (int count=0;count < argc-1; count++) {
    argv2[count] = argv[count+2];
  }

  for (int i=0;i<repeat;i++) {
    pid=fork();
    if (pid == 0) {
      execv(argv2[0], argv2);
    }
    else if (i > 0) {
      clock_gettime(CLOCK_MONOTONIC_RAW, &start);
      waitpid(pid, &stat, 0);
      clock_gettime(CLOCK_MONOTONIC_RAW, &end);
      time_used += (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 100000000;
    }
  }

  printf("----------Test result----------\n");
  printf("Total time used: %f\n", time_used);
  printf("Average time used: %f\n", time_used/repeat);
  printf("-------------------------------\n");

}
