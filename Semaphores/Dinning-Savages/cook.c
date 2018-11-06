#include "header.h"
#include <time.h>

void a_cook(char* program, int amount) {
  int semid, i, k;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(program);
    exit(-1);
  }

  if ( (semid = semget(key, 3, 0666))  < 0 ) {
    perror(program);
    exit(-1);
  }

  while(1) {
    mutex_wait(semid, EMPTYPOT);
    printf("The cook %i puts more food\n", getpid());

    sem_signal(semid, SERVINGS, amount);
    mutex_signal(semid, FULLPOT);
  }
}

int main(int argc, char* argv[]) {
  int amount = 0, semid, i, pid;
  key_t key;

  if(argc != 2) {
    printf("usage: %s amount\n", argv[0]);
    return -1;
  }

  amount = atoi(argv[1]);
  if(amount < 1) {
    printf("The amount must be a positive integer number\n");
    return -1;
  }

  a_cook(argv[0], amount);
  return 0;
}
