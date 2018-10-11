#include "header.h"
#include <time.h>

void a_customer(char* program, int amount) {
  int semid;
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
    printf("Customer %i arrives\n", getpid());
    mutex_wait(semid, MUTEX);
    int x = semctl(semid, CURRENTCUSTOMERS, GETVAL, 0);
    if(x == amount) {
      mutex_signal(semid, MUTEX);
      printf("Customer %i leaves the queue\n", getpid());
    }
    sem_signal(semid, CURRENTCUSTOMERS, 1);
    mutex_signal(semid, MUTEX);
    mutex_signal(semid, CUSTOMER);
    mutex_wait(semid, BARBER);
    printf("Customer %i is getting their haircut\n", getpid());
    mutex_signal(semid, CUSTOMERDONE);
    mutex_wait(semid, BARBERDONE);

    mutex_wait(semid, MUTEX);
    sem_wait(semid, CURRENTCUSTOMERS, 1);
    mutex_signal(semid, MUTEX);
  }
}

int main(int argc,char* argv[]) {
  int amount = 0, semid, i, pid, customers = 0;

  if(argc != 3) {
    printf("usage: %s amount customers\n", argv[0]);
    return -1;
  }

  amount = atoi(argv[1]);
  if(amount < 1) {
    printf("The amount must be a positive integer number\n");
    return -1;
  }

  customers = atoi(argv[2]);
  if(customers < 1) {
    printf("Customers must be a positive integer number\n");
    return -1;
  }

  for(i = 0; i < customers; i++) {
    if((pid = fork()) < 0) {
      perror("fork");
      return -1;
    } else if (pid == 0) {
      a_customer(argv[0], amount);
    } else {
    }
  }
  return 0;
}
