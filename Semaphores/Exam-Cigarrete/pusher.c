#include "header.h"

int main(int argc, char *argv[]){
  int semid, shmid, i;
  key_t key;
  struct buffer *b;

  if((key=ftok("/dev/null", 65))==(key_t) -1){
	perror(argv[0]);
	return -1;
  }

  if((semid=semget(key, 5, 0666))  < 0){
  	perror(argv[0]);
  	return -1;
  }

  if((shmid=shmget(key, sizeof(struct buffer), 0666)) < 0){
  	perror("shmid");
  	return -1;
  }

  b=(struct buffer *) shmat(shmid,(void *) 0, 0);

  srand( getpid());

  while(1){
  	i=rand() % 3;
    if(i==0){
      printf("Benji comes\n");
      b->billete100=1;
      printf("Benji puts a 20 dollar\n");
      sem_signal(semid, BILLETE20, 1);
      sem_wait(semid, BENJI, 1);
    } else if(i==1){
      printf("Benji comes\n");
      b->billete50=1;
      printf("Benji puts a 100 dollar\n");
      sem_signal(semid, BILLETE100, 1);
      sem_wait(semid, BENJI, 1);
    } else if(i==2){
      printf("Benji comes\n");
      b->billete20=1;
      printf("Benji puts a 50 dollar\n");
      sem_signal(semid, BILLETE50, 1);
      sem_wait(semid, BENJI, 1);
    }
  	printf("Benji goes\n");
  	sleep(2);
  }
  return 0;
}
