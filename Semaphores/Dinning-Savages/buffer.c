#include "header.h"

int main(int argc, char* argv[]) {
  int semid, i, size;
  unsigned short final_values[4];
  key_t key;

  if(argc != 2) {
    printf("Usage: %s size\n", argv[0]);
    return -1;
  }

  size = atoi(argv[1]);
  if(size < 1) {
    printf("%s: The size must be a positive number\n", argv[0]);
    return -1;
  }

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(argv[0]);
    return -1;
  }

  if( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0) {
    perror(argv[0]);
    return -1;
  }

  semctl(semid, SERVINGS, SETVAL, size);
  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, FULLPOT, SETVAL, 0);
  semctl(semid, EMPTYPOT, SETVAL, 0);

  semctl(semid, 0, GETALL, final_values);
  printf("values: \n");
  for(i = 0; i < 4; i++) {
    printf("%3i\n", final_values[i]);
  }
  printf("\n");
  return 0;
}
