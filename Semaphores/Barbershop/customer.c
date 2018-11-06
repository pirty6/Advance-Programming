/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 11-Oct-2018

* Autor: A01206747 Mariana Perez Garcia

*

*--------------------------------------------------------------*/
#include "header.h"
#include <time.h>

void a_customer(char* program, int timeReturn) {
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
    printf("Customer %i arrived at barber shop.\n", getpid());
    mutex_wait(semid, MUTEX);
    int x = semctl(semid, CURRENTCUSTOMERS, GETVAL, 0);
    int y = semctl(semid, TOTALCUSTOMER, GETVAL, 0);
    if(x == y) {
      mutex_signal(semid, MUTEX);
      printf("Customer %i leaves the queue\n", getpid());
    } else {
      printf("Customer %i entering waiting room.\n", getpid());
      sem_signal(semid, CURRENTCUSTOMERS, 1);
      mutex_signal(semid, MUTEX);
      mutex_signal(semid, CUSTOMER);
      mutex_wait(semid, BARBER);
      mutex_signal(semid, CUSTOMERDONE);
      mutex_wait(semid, BARBERDONE);
      mutex_wait(semid, MUTEX);
      printf("Customer %i leaving barber shop.\n", getpid());
    }
    sem_wait(semid, CURRENTCUSTOMERS, 1);
    int k = semctl(semid, CURRENTCUSTOMERS, GETVAL, 0);
    if(k == 0) {
      printf("The barber is sleeping\n");
    }
    mutex_signal(semid, MUTEX);
    sleep(timeReturn);
  }
}

int main(int argc,char* argv[]) {
  int amount = 0, semid, i, pid, customers = 0, timeReturn = 0;

  if(argc != 2) {
    printf("usage: %s customers time_to_return\n", argv[0]);
    return -1;
  }

  timeReturn = atoi(argv[1]);
  if(timeReturn < 1) {
    printf("time_to_return must be a positive integer number\n");
    return -1;
  }

  a_customer(argv[0], timeReturn);

  return 0;
}
