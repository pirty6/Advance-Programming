#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//Thread A has to wait for Thread B and vice versa.
//a1 < b2 and b1 < a2

sem_t aArrived, bArrived;

void* threadA(void* arg) {
  printf("a1\n");
  sem_post(&aArrived);
  sem_wait(&bArrived);
  sleep(2);
  printf("a2\n");
}

void* threadB(void* arg) {
  printf("b1\n");
  sem_post(&bArrived);
  sem_wait(&aArrived);
  sleep(2);
  printf("b2\n");
}

int main(int argc, char* argv[]) {
  sem_init(&aArrived, 0, 1);
  sem_init(&bArrived, 0, 1);
  pthread_t ta, tb;
  pthread_create(&ta, NULL, threadA, NULL);
  pthread_create(&tb, NULL, threadB, NULL);
  pthread_join(ta, NULL);
  pthread_join(tb, NULL);
  sem_destroy(&aArrived);
  sem_destroy(&bArrived);
  return 0;
}
