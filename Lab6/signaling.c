#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>

int number = 10;
int counter = 0;

void handler1(int signal) {
  counter++;
}

int main(int argc, char* argv[]) {
  if(argc > 2) {
    printf("Usage: %s [n]\n", argv[0]);
    return -1;
  }
  if(argc == 2) {
    number = atoi(argv[1]);
    if(number <= 0 || number != atof(argv[1])) {
      printf("%s: The parameter must be a positive integer number greater than zero.\n", argv[0]);
      return -1;
    }
  }

  while(counter < number) {
    int pid = strtol(argv[1], NULL, 10);
    kill(pid, SIGUSR1);
    signal(SIGUSR1, handler1);
  }
  signal(SIGUSR2, handler2);

  return 0;
}
