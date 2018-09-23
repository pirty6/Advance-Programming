/*----------------------------------------------------------------

 *

 * Programación avanzada: Creación de procesos.

 * Fecha: 18-Sep-2018

 * Autor: A01206747 Mariana Perez Garcia

 *

 --------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process(char* program, int level, int lim){
	int i, pid;

	for (i = 0; i < level; i++){
		printf("  ");
	}

	printf("PPID = %i PID = %i LEVEL= %i\n", getppid(), getpid(), level);

	if(level == lim){
		exit(0);
	}

	for(i = 0; i <=	 level; i++){
		if((pid = fork()) < 0){
			perror(program);
			exit(-1);
		}else if(pid == 0){
			child_process(program, ++level, lim);
		}else{
			wait(NULL);
		}
	}

	exit(0);
}

int main(int argc, char* argv[]){

	int pid, num = atoi(argv[1]);

	if(argc != 2){
		fprintf(stderr,"Usage: %s [LEVEL]\n",argv[0]);
		return -1;
	}else{
		if(num <= 0 || num != atof(argv[1])){
			printf("The parameter must be a positive integer greater than 0\n");
			return -1;
		}
	}

	printf("PPID = %i PID = %i LEVEL= %i\n", getppid(), getpid(), 0);

	if((pid = fork()) < 0){
		perror(argv[0]);
		exit(-1);
	}else if(pid == 0){
		child_process(argv[0], 1, num);
	}else{
		wait(NULL);
	}

	return 0;
}
