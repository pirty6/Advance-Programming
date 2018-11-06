#include "header.h"
#include <time.h>

void a_santa(char* program) {
  int semid, i;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(program);
    exit(-1);
  }

  if ( (semid = semget(key, 6, 0666))  < 0 ) {
    perror(program);
    exit(-1);
  }

	while (1) {
    printf("Santa %i is sleeping\n", getpid());
    mutex_wait(semid, SANTASEM);
    mutex_wait(semid, MUTEX);
    int reindeer = semctl(semid, REINDEER, GETVAL, 0);
    int elves = semctl(semid, ELVES, GETVAL, 0);
    if(reindeer >= 9) {
      printf("Santa %i is preparing the sleigh\n", getpid());
      sem_signal(semid, REINDEERSEM, 9);
      sem_wait(semid, REINDEER, 9);
    } else if (elves == 3){
      printf("Santa %i is helping the elves\n", getpid());
    }
    mutex_signal(semid, MUTEX);
    sleep(1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {

  if(argc != 1) {
    printf("usage: %s\n", argv[0]);
    return -1;
  }

  a_santa(argv[0]);

  return 0;
}
