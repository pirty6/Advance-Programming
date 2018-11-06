#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//Thread B has to wait for Thread A.

sem_t sem;

void* threadA(void* arg) {
  printf("Im thread A!\n");
  sem_post(&sem);
}

void* threadB(void* arg) {
  sem_wait(&sem);
  printf("Im thread B!\n");
}

int main(int argc, char* argv[]) {
  sem_init(&sem, 0, 0);
  pthread_t ta, tb;
  pthread_create(&ta, NULL, threadA, NULL);
  pthread_create(&tb, NULL, threadB, NULL);
  pthread_join(ta, NULL);
  pthread_join(tb, NULL);
  sem_destroy(&sem);
  return 0;
}
