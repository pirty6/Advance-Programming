#include "header.h"

int main(int argc, char* argv[]) {
  int semid, i;
  unsigned short final_values[3];
  key_t key;

  if(argc != 1) {
    printf("usage: %s\n", argv[0]);
		return -1;
  }

  if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

  if ( (semid = semget(key, 3, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

  semctl(semid, READER, SETVAL, 1);
  semctl(semid, WRITER, SETVAL, 1);
  semctl(semid, ROOMEMPTY, SETVAL, 0);

  semctl(semid, 0, GETALL, final_values);
  printf("values: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");
	return 0;
}
