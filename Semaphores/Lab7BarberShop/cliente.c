/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 11-Oct-2018

* Autor: A01207499 Cesar Buenfil Vazquez

*

*----------------------------------------------------------------*/
#include "header.h"
#include <unistd.h>

void crear_cliente(int t) {
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

	printf("\tPID = %i Cliente: Nuevo cliente\n", getpid());
	mutex_wait(semid, MUTEX);

	int num_clientes = semctl(semid, CLIENTE, GETVAL);
	if ( num_clientes > semctl(semid, SILLAS, GETVAL)) {
    printf("Cliente: La barbería está llena, ya me voy\n");
		exit(0);
	}
	mutex_signal(semid, MUTEX);

	sem_signal(semid, CLIENTE, 1);

	printf("Cliente: Esperando...\n");
	int count = 0;
	while(mutex_wait(semid, CORTEPELO) == -1){
		sleep(1);
		count++;
		if(count > t){
			printf("El bato se mamo.jpg\n");
			exit(0);
		}
	}


  printf("Cliente: Me están cortando el cabello\n");
	sleep(5);
  printf("Cliente: Ya me cortaron el cabello\n");
	sleep(3);
	printf("Cliente: Me retiro\n");

	exit(0);
}

int main(int argc, char* argv[]) {
	int pid;

	if(argc != 2){
		printf("Usage: clientes [tiempo]\n");
		return -1;
	}

	else{
		int tiempo = atoi(argv[1]);
		if(tiempo > 0){
			if ( (pid = fork()) == 0 ) {
				 crear_cliente(tiempo);
			}
			else if (pid < 0) {
				printf("Error al crear el proceso\n");
				return -1;
			}
		}
		else{
			printf("El tiempo de cada cliente debe ser mayor a 0\n");
		}
	}
	return 0;
}
