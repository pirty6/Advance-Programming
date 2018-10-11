#include "header.h"
#include <time.h>

void a_barber(char* program) {
  int semid, i, k;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(program);
    exit(-1);
  }

  if ( (semid = semget(key, 7, 0666))  < 0 ) {
    perror(program);
    exit(-1);
  }

  while(1) {
    printf("The barber %i goes to sleep\n", getpid());
    mutex_wait(semid, CUSTOMER);
    mutex_signal(semid, BARBER);
    printf("The barber %i is cutting the hair\n", getpid());
    mutex_signal(semid, CUSTOMERDONE);
    mutex_signal(semid, BARBERDONE);
    sleep(2);
  }
}


int main(int argc, char* argv[]) {
  int amount = 0;
  key_t key;

  if(argc != 1) {
    printf("usage: %s\n", argv[0]);
    return -1;
  }

  a_barber(argv[0]);
  return 0;
}
