/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 11-Oct-2018

* Autor: A01206747 Mariana Perez Garcia

*

*--------------------------------------------------------------*/
#include "header.h"
#include <time.h>

void a_barber(char* program, int amount) {
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
    int y = semctl(semid, CURRENTCUSTOMERS, GETVAL, 0);
    if(y == 0){
      printf("The barber %i is sleeping\n", getpid());
    }
    mutex_wait(semid, CUSTOMER);
    mutex_signal(semid, BARBER);
    printf("The barber %i is cutting the hair\n", getpid());
    sleep(3);
    printf("The barber %i has finished cutting hair.\n", getpid());
    mutex_wait(semid, CUSTOMERDONE);
    mutex_signal(semid, BARBERDONE);
    int x = semctl(semid, CURRENTCUSTOMERS, GETVAL, 0);
  }
}


int main(int argc, char* argv[]) {
  int amount = 0;
  key_t key;

  if(argc != 2) {
    printf("usage: %s chairs\n", argv[0]);
    return -1;
  }

  amount = atoi(argv[1]);
  if(amount < 1) {
    printf("Chairs must be a positive integer number\n");
    return -1;
  }

  a_barber(argv[0], amount);
  return 0;
}
