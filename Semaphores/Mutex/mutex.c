#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//In order for a thread to access a shared variable, it has to "get" the mutex,
//when it is done it "releases" the mutex

sem_t mutex;
int counter;

void* threadA(void* arg) {
  //printf("Counter before tA: %i\n", counter);
  sem_wait(&mutex);
  counter++;
  printf("Counter after tA: %i\n", counter);
  sem_post(&mutex);
}

void* threadB(void* arg) {
  //printf("Counter before tB: %i\n", counter);
  sem_wait(&mutex);
  counter++;
  printf("Counter after tB: %i\n", counter);
  sem_post(&mutex);
}

int main(int argc, char* argv[]) {
  sem_init(&mutex, 0, 1);
  pthread_t ta, tb;
  printf("Inicial Counter: %i\n", counter);
  pthread_create(&ta, NULL, threadA, NULL);
  pthread_create(&tb, NULL, threadB, NULL);
  pthread_join(ta, NULL);
  pthread_join(tb, NULL);
  sem_destroy(&mutex);
  return 0;
}
