#include "header.h"

int main(int argc, char* argv[]) {
  int semid, i, size;
  unsigned short final_values[7];
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

  if( (semid = semget(key, 7, 0666 | IPC_CREAT)) < 0) {
    perror(argv[0]);
    return -1;
  }

  semctl(semid, TOTALCUSTOMER, SETVAL, size);
  semctl(semid, CURRENTCUSTOMERS, SETVAL, 0);
  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, CUSTOMER, SETVAL, 0);
  semctl(semid, BARBER, SETVAL, 0);
  semctl(semid, CUSTOMERDONE, SETVAL, 0);
  semctl(semid, BARBERDONE, SETVAL, 0);

  semctl(semid, 0, GETALL, final_values);
  printf("values: \n");
  for(i = 0; i < 7; i++) {
    printf("%3i\n", final_values[i]);
  }
  return 0;
}
