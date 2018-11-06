#include "header.h"

void a_reader(char* program) {
  int semid, i, k;
  key_t key;

  if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror(program);
		exit(-1);
	}

  srand( getpid() );
  k = 10;
  while(k-- > 0) {
    printf("Reader %i trying to access the buffer.\n", getpid());
    mutex_wait(semid, READER);

  }

}

int main(int argc, char* argv[]) {
  int semid, amount = 0, i, pid;
  key_t key;

  if(argc != 2) {
    printf("usage: %s amount\n", argv[0]);
  }

  amount = atoi(argv[1]);
	if (amount < 1) {
		printf("%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

  for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_reader(argv[0]);
		} else {
		}
	}

  return 0;
}
