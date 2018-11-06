#include "header.h"
#include <time.h>

void a_reindeer(char* program) {
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
    mutex_wait(semid, MUTEX);
    sem_signal(semid, REINDEER, 1);
    printf("A reindeer has arrived\n");
    int reindeer = semctl(semid, REINDEER, GETVAL, 0);
    if (reindeer == 9) {
      printf("There are 9 deers\n");
      mutex_signal(semid, SANTASEM);
    }
    mutex_signal(semid, MUTEX);
    mutex_wait(semid, REINDEERSEM);
    printf("The reindeer go to hitch\n");
    sleep(1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {

  if(argc != 1) {
    printf("usage: %s\n", argv[0]);
    return -1;
  }

  a_reindeer(argv[0]);

  return 0;
}
