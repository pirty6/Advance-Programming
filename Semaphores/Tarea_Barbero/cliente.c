#include "header.h"

void client(int client_id, int periodo_de_regreso, char* program){
  printf("Cliente %i: %i\n", client_id,getpid());
  int i, k, semid, shmid;
  key_t key;
  Buffer *b;

  if( (key=ftok("vacio", 125)) ==(key_t)-1 ){
    perror(program);
    exit(-1);
  }

  if( (semid=semget(key, 5, 0666))<0 ){
    perror(program);
    exit(-1);
  }

  if ( (shmid = shmget(key, sizeof(Buffer), 0666)) < 0 ) {
      perror(program);
      exit(-1);
  }

  b = (Buffer*) shmat(shmid, (void*) 0, 0);


while(1){
  printf("Buffer info: customers: %i chairs: %i \n", b->customers, b->chairs );

  sem_wait(semid, MUTEX, 1); //restandole 1 a MUTEX poniendo el lock

    if ( b->customers < b->chairs){
      printf("%i: encontré una silla vacia\n", getpid());
      printf("Buffer info: customers: %i chairs: %i \n", b->customers, b->chairs );

      b->customers++;
      sem_signal(semid, MUTEX, 1); //libero el mutex
      sem_signal(semid, CUSTOMERS, 1); //libero CUSTOMERS, inicialmente estaba bloqueado, sumo 1
      sem_wait(semid, BARBER, 1); //restandole 1 a MUTEX poniendo el lock

      printf("%i: me están cortando el cabello\n", getpid());

      sem_signal(semid, CUSTOMER_DONE, 1); //libero el CUSTOMER_DONE
      sem_wait(semid, BARBER_DONE, 1); //lock a BARBER

      sem_wait(semid, MUTEX, 1); //restandole 1 a MUTEX poniendo el lock
      b->customers=b->customers-1; //modifico customers
      printf("%i: ya me cortaron el cabello, iré a dormir\n", getpid());

      sem_signal(semid, MUTEX, 1); //libero el mutex
      printf("Buffer info: customers: %i chairs: %i \n", b->customers, b->chairs );


    }
    else{
      //el cliente se duerme
      sem_signal(semid, MUTEX, 1); //libero el mutex
      printf("%i: no había sillas, iré a dormir\n",getpid());
    }
    sleep(periodo_de_regreso);
  }
  exit(0);
}


int main(int argc, char* argv[]){
  int num_clients, periodo_de_regreso, i, pid, shmid;
  key_t key;

  if(argc!=3){
    fprintf(stderr, "usage:%s  periodo_de_regreso num_de_clientes\n",argv[0] );
    return -1;
  }


  if((periodo_de_regreso = atoi(argv[1]))<1){
    fprintf(stderr, "%s: The amount of time must be greater or equal to 1", argv[0]);
    return -1;
  }

  if((num_clients = atoi(argv[2]))<1){
    fprintf(stderr, "%s: The number of clients must be greater or equal to 1", argv[0]);
    return -1;
  }


  for(i=0; i<num_clients; i++){
    if( (pid=fork())<0 ){
      perror(argv[0]);
      return -1;
    }
    else if(pid==0){
      client( i, periodo_de_regreso, argv[0] );
    }
    else{
      //do nothing
    }
  }


}
