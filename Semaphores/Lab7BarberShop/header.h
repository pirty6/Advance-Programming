/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 11-Oct-2018

* Autor: A01207499 Cesar Buenfil Vazquez

*

*----------------------------------------------------------------*/
#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define	CLIENTE			0
#define SILLA				1
#define MUTEX				2
#define CORTEPELO		3
#define SILLAS	    4
#define SEMAFOROS		5

int sem_wait(int semid, int num, int valor) {
	struct sembuf op;

	op.sem_num = num;
	op.sem_op = -valor;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int num, int valor) {
	struct sembuf op;

	op.sem_num = num;
	op.sem_op = valor;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int mutex_wait(int semid, int num) {
	return sem_wait(semid, num, 1);
}

int mutex_signal(int semid, int num) {
	return sem_signal(semid, num, 1);
}

#endif
