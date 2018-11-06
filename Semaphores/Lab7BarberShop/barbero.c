/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 11-Oct-2018

* Autor: A01207499 Cesar Buenfil Vazquez

*

*----------------------------------------------------------------*/
#include "header.h"
#include <unistd.h>

void crear_barbero() {
	int semid, i;
	key_t k;

	if ( (k = ftok("/dev/null", 65)) == (key_t) -1 ) {
		printf("Error en ket_t");
		exit(-1);
	}

	if ( (semid = semget(k, SEMAFOROS, 0666))  < 0 ) {
        printf("Error en el semaforo");
		exit(-1);
	}

	while(1) {
    printf("\tPID = %i Barbero: ZzZzZz \n", getpid());
		sem_wait(semid, CLIENTE, 1);
  	printf("Barbero: Estoy cortando el cabello\n");
		sleep(5);
    printf("Barbero: Terminé\n");
		sleep(1);
		mutex_signal(semid, CORTEPELO);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int pid;
	if ( (pid = fork()) == 0 ) {
	 crear_barbero();
	}
	else if (pid < 0) {
    printf("Error al crear el proceso");
		return -1;
	}
return 0;
}
