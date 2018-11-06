#include "header.h"

void reindeers( char* program){
  int semid, shmid;
  key_t key;
  Buffer *b;

  if( (key=ftok("vacio", 125)) ==(key_t)-1 ){
    printf("error en key");
    perror(program);
    exit(-1);
  }

  if( (semid=semget(key, 4, 0666))<0 ){
    printf("error en semid");

    perror(program);
    exit(-1);
  }

  if ( (shmid = shmget(key, sizeof(Buffer), 0666)) < 0 ) {
    printf("error en shmid");

      perror(program);
      exit(-1);
  }

  b = (Buffer*) shmat(shmid, (void*) 0, 0);

  sem_wait(semid, MUTEX, 1); //restandole 1 a MUTEX poniendo el lock
  b->reindeer+=1;

  printf("new reindeer\n");
  printf("Memory: reindeers: %i, elves: %i\n", b->reindeer, b->elves);
  if(b->reindeer==9){
    sem_signal(semid, SANTA, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1
  }
  sem_signal(semid, MUTEX, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1

  sem_wait(semid, REINDEER, 1); //restandole 1 a MUTEX poniendo el lock
  printf("Reindeer: Get hitched\n");

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
      reindeers(argv[0] );
    }
    else{
      //do nothing
    }
  }

}
