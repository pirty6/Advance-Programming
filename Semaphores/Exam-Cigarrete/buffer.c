/*
	Cigarrete Smokers
	benji es el agente y los ottakus los fumadores y los billetes los materiaes

*/
#include "header.h"

int main(int argc, char *argv[]){
    int semid, shmid;
    key_t key;
    struct buffer *b;

    if((key=ftok("/dev/null", 65))==(key_t) -1){
		perror(argv[0]);
		return -1;
	}

	if((semid=semget(key, 5, 0666 | IPC_CREAT))  < 0){
		perror(argv[0]);
		return -1;
	}

	semctl(semid, BILLETE100, SETVAL, 0);
	semctl(semid, BILLETE50, SETVAL, 0);
	semctl(semid, BILLETE20, SETVAL, 0);
	semctl(semid, BENJI, SETVAL, 0);

	if((shmid=shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0){
		semctl(semid, 0, IPC_RMID, 0);
		return -1;
	}
	b=(struct buffer *) shmat(shmid,(void *) 0, 0);

    return 0;
}
