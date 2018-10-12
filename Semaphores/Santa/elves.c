#include "header.h"

void elves( char* program){

  int semid, shmid;
  key_t key;
  Buffer *b;

  if( (key=ftok("vacio", 125)) ==(key_t)-1 ){
    perror(program);
    exit(-1);
  }

  if( (semid=semget(key, 4, 0666))<0 ){
    perror(program);
    exit(-1);
  }

  if ( (shmid = shmget(key, sizeof(Buffer), 0666)) < 0 ) {
      perror(program);
      exit(-1);
  }

  b = (Buffer*) shmat(shmid, (void*) 0, 0);

  sem_wait(semid, ELVES, 1); //restandole 1 a MUTEX poniendo el lock
  sem_wait(semid, MUTEX, 1); //restandole 1 a MUTEX poniendo el lock
  b->elves+=1;
  printf("elve created\n");
  printf("Memory: reindeers: %i, elves: %i\n", b->reindeer, b->elves);
  if(b->elves==3){
    sem_signal(semid, SANTA, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1
    printf("Elves: Get helped\n");
    //b->elves-=1;
    if(b->elves==0){
      sem_signal(semid, ELVES, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1

    }
    sem_signal(semid, MUTEX, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1

  }
  else{
    sem_signal(semid, ELVES, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1
    sem_signal(semid, MUTEX, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1

  }


  exit(0);
}


int main(int argc, char* argv[]){
  int i, pid;
  key_t key;

  for(i=0; i<9; i++){
    if( (pid=fork())<0 ){
      perror(argv[0]);
      return -1;
    }
    else if(pid==0){
      elves(argv[0] );
    }
    else{
      //do nothing
    }
  }

}
