/*----------------------------------------------------------------

*

* Programación avanzada: Proyecto 2

* Fecha: 6 Nov 2018

* Autor: A01206747 Mariana Perez

*

*--------------------------------------------------------------*/
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include<pthread.h> //for threading , link with lpthread

#define HELLO       1
#define SEND		    101
#define LS		      102
#define END		      103
#define DENIED      201
#define NOTFOUND	  202
#define ERROR       203
#define UNKNOWN     204
#define DIRECTORY   205
#define SENDFILE    301
#define DIRNOTFOUND 206
#define NOTDIR      207
#define SENDDIR     302

#define MUTEX       0

#define HOLA "hola"
#define SNOTFOUND "Archivo no encontrado"
#define ERRORINTERNO "Error interno"
#define RDIRECTORY "La ruta es un directorio"
#define PDENIED "Permiso denegado"
#define DNOTFOUND "Directorio no encontrado"
#define RNOTDIRECTORY "La ruta no es un directorio"
#define UNKNOWNCOMMAND "Comando no conocido"

int mutex_wait(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

#endif
