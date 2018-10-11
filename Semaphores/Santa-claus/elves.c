#include "header.h"
#include <time.h>

void a_elves(char* program) {
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
    mutex_wait(semid, ELFTEX);
    mutex_wait(semid, MUTEX);
    sem_signal(semid, ELVES, 1);
    printf("An elf has arrived\n");
    int elves = semctl(semid, ELVES, GETVAL, 0);
    if(elves == 3) {
      printf("There are 3 elves\n");
      mutex_signal(semid, SANTASEM);
    } else {
      mutex_signal(semid, ELFTEX);
    }
    mutex_signal(semid, MUTEX);
    printf("The elves went for help\n");
    mutex_wait(semid, MUTEX);
    printf("An elf went away\n");
    sem_wait(semid, ELVES, 1);
    elves = semctl(semid, ELVES, GETVAL, 0);
    if (elves == 0) {
      mutex_signal(semid, ELFTEX);
      printf("The next batch of elves entered\n");
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

  a_elves(argv[0]);

  return 0;
}
