/*----------------------------------------------------------------

*

* Programación avanzada: Manejando señales

* Fecha: 23-Sep-2015

* Autor: A1206747 Mariana Perez

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>

int number = 10;
int counter = 0;
int flag = 0;

void handler(int signal) {
  switch(signal) {
    case SIGUSR2 :
      if (flag == 0) {
        counter++;
        printf("SIGUSR2 - cont = %i - n = %i\n", counter, number);
      }
      break;
    case SIGUSR1 :
      if(flag == 0) {
        flag = 1;
        printf("SIGUSR1 - SIGUSR2 is now ignored\n");
      } else {
        printf("SIGUSR1 - SIGUSR2 is now accepted\n");
        flag = 0;
      }
      break;
  }
}

int main(int argc, char* argv[]) {


  if(argc > 2) {
    printf("Usage: %s [n]\n", argv[0]);
    return -1;
  }
  if(argc == 2) {
    number = atoi(argv[1]);
    if(number <= 0 || number != atof(argv[1])) {
      printf("%s: The parameter must be a positive integer number greater than zero.\n", argv[0]);
      return -1;
    }
  }

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

  while(counter < number) {
    sleep(30);
  }
  printf("Exiting...\n");

  return 0;
}
