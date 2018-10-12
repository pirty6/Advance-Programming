#include "header.h"
#include <time.h>

int d20 = 0;
int d50 = 0;
int d100 = 0;

void a_pusher(char* program) {
  int semid, i;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(program);
    exit(-1);
  }

  if ( (semid = semget(key, 3, 0666))  < 0 ) {
    perror(program);
    exit(-1);
  }

	while (1) {
    mutex_wait(semid, DOLLAR20);
    mutex_wait(semid, MUTEX);
    if(d20 == 1) {
      d20 = 0;
      mutex_signal(semid, D50SEM);
    } else if (d50 == 1) {
      d50 = 0;
      mutex_signal(semid, D100SEM);
    } else {
      d100 = 1;
    }
    mutex_signal(semid, MUTEX);

	}
	exit(0);
}

int main(int argc, char* argv[]) {

  if(argc != 1) {
    printf("usage: %s\n", argv[0]);
    return -1;
  }

  a_pusher(argv[0]);
  return 0;
}
