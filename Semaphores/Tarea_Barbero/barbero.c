#include "header.h"

void barbero(char* program){
  int i, k, semid;
  key_t key;

  if( (key=ftok("vacio", 125)) ==(key_t)-1 ){
    perror(program);
    exit(-1);
  }

  if( (semid=semget(key, 5, 0666))<0 ){
    perror(program);
    exit(-1);
  }
  while(1){

    sem_wait(semid, CUSTOMERS, 1); //restandole 1 a CUSTOMERS poniendo el lock
    sem_signal(semid, BARBER, 1); //libero el BARBER
      //cutting hair
    printf("Barbero: Estoy cortando cabello\n");
    sem_wait(semid, CUSTOMER_DONE, 1); //restandole 1 a CUSTOMER_DONE poniendo el lock
    sem_signal(semid, BARBER_DONE, 1); //libero el BARBER_DONE
  }
}

int main(int argc, char* argv[]){

    if(argc!=1){
      fprintf(stderr, "usage:%s \n", argv[0]);
      return -1;
    }
  barbero(argv[0]);
}
