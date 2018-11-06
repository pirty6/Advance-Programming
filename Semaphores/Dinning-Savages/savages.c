#include "header.h"
#include <time.h>

void a_savage(char* program) {
  int semid, i;
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
    printf("Savage %i is sleeping.\n", getpid());
    mutex_wait(semid, MUTEX);
    int x = semctl(semid, SERVINGS, GETVAL, 0);
    if (x == 0) {
      printf("Savage goes to the cook%i\n", getpid());
      mutex_signal(semid, EMPTYPOT);
      printf("Savage %i goes back to sleep.\n", getpid());
      mutex_wait(semid, FULLPOT);
    }
    printf("Savage %i is eating.\n", getpid());
    sem_wait(semid, SERVINGS, 1);
    mutex_signal(semid, MUTEX);
	}
	exit(0);
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

  for(i = 0; i < amount; i++) {
    if((pid = fork()) < 0) {
      perror("fork");
      return -1;
    } else if (pid == 0) {
      a_savage(argv[0]);
    } else {
    }
  }
  return 0;
}
