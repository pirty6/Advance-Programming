#include "header.h"



int main(int argc, char* argv[]){
    int i, semid, shmid;
    unsigned short final_values[4];
    key_t key;
    Buffer *b;

    if ((key=ftok("vacio",125))==(key_t) -1){ //para poder guardar los recursos disponibles
      printf("KEY");
      perror(argv[0]);
      return -1;
    }


    if ((semid=semget(key,4,0666|IPC_CREAT))<0){ //crea 4 semaforos
      perror(argv[0]);
      return -1;
    }

    semctl(semid, SANTA, SETVAL, 0);
    semctl(semid, REINDEER, SETVAL, 0);
    semctl(semid, ELVES, SETVAL, 1);
    semctl(semid, MUTEX, SETVAL, 1);


    //Usar solo para debuggear

    semctl(semid, MUTEX, GETALL, final_values);
    printf("value: ");

    for(i=0; i<4; i++){
      printf("%3i\n",final_values[i] );
    }
    printf("\n");

    if ( (shmid = shmget(key, sizeof(Buffer), 0666|IPC_CREAT)) < 0 ) {
        printf("SHMID");
        perror(argv[0]);
        exit(-1);
    }


    b = (Buffer*) shmat(shmid, (void*) 0, 0);

    b->elves=0;
    b->reindeer=0;

    while(1){
      sem_wait(semid, SANTA, 1); //restandole 1 a MUTEX poniendo el lock
      sem_wait(semid, MUTEX, 1); //restandole 1 a MUTEX poniendo el lock
        if(b->reindeer>=9){
          printf("Memory: reindeers: %i, elves: %i\n", b->reindeer, b->elves);
          printf("Prepare Sleigh\n");
          sem_signal(semid, REINDEER, 9); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1
          b->reindeer-=9;
        }
        else if(b->elves==3){
          printf("Memory: reindeers: %i, elves: %i\n", b->reindeer, b->elves);
          printf("helping elves\n");
        }
        sem_signal(semid, MUTEX, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1

    }

    return 0;
}
