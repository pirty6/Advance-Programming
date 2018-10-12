#include "header.h"

int main(int argc, char* argv[]) {
  int semid, i, size;
  unsigned short final_values[8];
  key_t key;

  if(argc != 1) {
    printf("Usage: %s\n", argv[0]);
    return -1;
  }

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(argv[0]);
    return -1;
  }

  if( (semid = semget(key, 8, 0666 | IPC_CREAT)) < 0) {
    perror(argv[0]);
    return -1;
  }

  semctl(semid, DOLLAR20, SETVAL, 0);
  semctl(semid, DOLLAR50, SETVAL, 0);
  semctl(semid, DOLLAR100, SETVAL, 0);
  semctl(semid, BENJI, SETVAL, 1);
  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, D20SEM, SETVAL, 0);
  semctl(semid, D50SEM, SETVAL, 0);
  semctl(semid, D100SEM, SETVAL, 0);

  semctl(semid, 0, GETALL, final_values);
  printf("values: \n");
  for(i = 0; i < 8; i++) {
    printf("%3i\n", final_values[i]);
  }
  printf("\n");
  return 0;
}
