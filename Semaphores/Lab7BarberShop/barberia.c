/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 11-Oct-2018

* Autor: A01207499 Cesar Buenfil Vazquez

*

*----------------------------------------------------------------*/
#include "header.h"

int main (int argc, char* argv[]) {
	int semid;
	unsigned short num_sem[SEMAFOROS];
	key_t key;

	if(argc!=2)
	{
		printf("Usage: barberia [numero_sillas]\n");
		return -1;
	}

	int sillas = atoi(argv[1]);

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		printf("Error en ket_t");
		return -1;
	}

	if ( (semid = semget(key, SEMAFOROS, 0666 | IPC_CREAT))  < 0 ) {
        printf("Error en el semaforo");
		return -1;
	}

	semctl(semid, CLIENTE, SETVAL, 0);
	semctl(semid, SILLA, SETVAL, 1);
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, CORTEPELO, SETVAL, 0);
	semctl(semid, SILLAS, SETVAL, sillas);
	semctl(semid, 0, GETALL, num_sem);



	return 0;
}
