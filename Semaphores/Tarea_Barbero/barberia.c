#include "header.h"

int main(int argc, char* argv[]){

    int chairs;
    int i, semid, size, shmid;
    unsigned short final_values[5];
    key_t key;
    Buffer *b;

    if(argc!=2){
      fprintf(stderr, "usage:%s cantidad_sillas\n",argv[0] );
      return -1;
    }

    if((chairs = atoi(argv[1]))<1){
      fprintf(stderr, "%s: The amount of chairs must be greater or equal to 1", argv[0]);
      return -1;
    }

    if ((key=ftok("vacio",125))==(key_t) -1){ //para poder guardar los recursos disponibles
      printf("KEY");
      perror(argv[0]);
      return -1;
    }

    if ((semid=semget(key,5,0666|IPC_CREAT))<0){ //crea 5 semaforos
      printf("SEMGET");
      perror(argv[0]);
      return -1;
    }

    if ( (shmid = shmget(key, sizeof(Buffer), 0666|IPC_CREAT)) < 0 ) {
        printf("SHMID");
    		perror(argv[0]);
    		exit(-1);
    }

    b = (Buffer*) shmat(shmid, (void*) 0, 0);

    b->customers=0;
    b->chairs=chairs;

    shmdt(b);//deattach

    semctl(semid, MUTEX, SETVAL, 1);
    semctl(semid, CUSTOMERS, SETVAL, 0);
    semctl(semid, BARBER, SETVAL, 0);
    semctl(semid, CUSTOMER_DONE, SETVAL, 0);
    semctl(semid, BARBER_DONE, SETVAL, 0);

    //Usar solo para debuggear

    semctl(semid, MUTEX, GETALL, final_values);
    printf("value: ");

    for(i=0; i<5; i++){
      printf("%3i\n",final_values[i] );
    }
    printf("\n");
    return 0;

}
