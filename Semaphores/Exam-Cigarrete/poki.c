#include "header.h"
#include <time.h>

void a_savage(char* program, int i) {
  int semid;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(program);
    exit(-1);
  }

  if ( (semid = semget(key, 4, 0666))  < 0 ) {
    perror(program);
    exit(-1);
  }

  while (1) {
    printf("Ottaku %i is waitings.\n", getpid());
    mutex_wait(semid, D20SEM);
    // mutex_wait(semid, DOLLAR50);
    // mutex_wait(semid, DOLLAR100);
    printf("Ottaku %i is buying a pokki.\n", getpid());
    mutex_signal(semid, BENJI);
    printf("Ottaku %i is now eating a pokki.\n", getpid());
  }
	exit(0);
}

int main(int argc, char* argv[]) {
  int semid, i, pid;
  key_t key;

  if(argc != 1) {
    printf("usage: %s\n", argv[0]);
    return -1;
  }

  for(i = 0; i < 3; i++) {
    if((pid = fork()) < 0) {
      perror("fork");
      return -1;
    } else if (pid == 0) {
      a_savage(argv[0], i);
    } else {
    }
  }
  return 0;
}
