
#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct buffer{
	int billete100;
	int billete50;
	int billete20;
};


#define BILLETE100 	  0
#define BILLETE50    	1
#define BILLETE20   	2
#define BENJI         4

int sem_wait(int semid, int sem_num, int val){
	struct sembuf op;

	op.sem_num=sem_num;
	op.sem_op=-val;
	op.sem_flg=0;
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val){
	struct sembuf op;

	op.sem_num=sem_num;
	op.sem_op=val;
	op.sem_flg=0;
	return semop(semid, &op, 1);
}

int mutex_wait(int semid, int sem_num){
	return sem_wait(semid, sem_num, 1);
}

int mutex_signal(int semid, int sem_num){
	return sem_signal(semid, sem_num, 1);
}

#endif
