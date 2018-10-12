#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>//semaforos
#include <sys/shm.h>

#define SANTA      0
#define REINDEER   1
#define ELVES      2
#define MUTEX      3


typedef struct buffer{

  int elves;
  int reindeer;

} Buffer; //alias



int sem_wait(int semid, int semnum, unsigned int val){
  struct sembuf op; //estructura para semaforos

  op.sem_num=semnum;
  op.sem_op=-val; //decrementar val
  op.sem_flg=0;

  return semop(semid, &op, 1); //unica manera de modificar un semaforo
}


int sem_signal(int semid, int semnum, unsigned int val){
  struct sembuf op; //estructura para semaforos

  op.sem_num=semnum;
  op.sem_op = val; //incremento val
  op.sem_flg=0;

  return semop(semid, &op, 1); //unica manera de modificar un semaforo
}

#endif
