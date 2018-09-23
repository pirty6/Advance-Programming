/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

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
#include <string.h>
#include <errno.h>
#include <ctype.h>

void findlog(char* name, char* directory, char* program, int secs, int count) {
  char filename[PATH_MAX + NAME_MAX + 1];
  char newfilename[PATH_MAX + NAME_MAX + 1];
  int i = 0, aux;
  for(i = 0; i < count; i++) {
    sleep(secs);
    for(aux = i; aux > 0; aux --) {
      int number = aux;
      char ext1[10];
      char ext2[10];
      if(directory[0] == '.') {
        sprintf(filename, "%s%s.%i", directory, name, number);
        sprintf(newfilename, "%s%s.%i", directory, name, number+1);
      } else {
        sprintf(filename, "%s/%s.%i", directory, name, number);
        sprintf(newfilename, "%s/%s.%i", directory, name, number+1);
      }
      rename(filename, newfilename);
    }
    if(directory[0] == '.') {
      sprintf(filename, "%s%s.log", directory, name);
      sprintf(newfilename, "%s%s.1", directory, name);
    } else {
      sprintf(filename, "%s/%s.log", directory, name);
      sprintf(newfilename, "%s/%s.1", directory, name);
    }
    rename(filename, newfilename);
    FILE *f;
    f = fopen(filename, "w");
    fclose(f);
  }
}



int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: %s working_directory log_filename number_of_files timeout\n", argv[0]);
    return -1;
  }
  char* p;
  char* q;
  char dir_name[NAME_MAX+1];
  char* directory;
  char filename[PATH_MAX + NAME_MAX + 1];
  getcwd(dir_name, NAME_MAX);
  directory = dir_name;
  directory = argv[1];
  errno = 0;
  int secs = strtol(argv[4], &p, 10);
  int count = strtol(argv[3], &q, 10);
  if (errno != 0 || *p != '\0' || secs > INT_MAX || secs < 0) {
    printf("Timeout must be a positive integer number\n");
    return -1;
  }
  if (errno != 0 || *q != '\0' || count > INT_MAX || count < 0) {
    printf("Number of files must be a positive integer number\n");
    return -1;
  }
  FILE *f;
  if(directory[0] == '.') {
    sprintf(filename, "%s%s.log", directory, argv[2]);
  } else {
    sprintf(filename, "%s/%s.log", directory, argv[2]);
  }
  f = fopen(filename, "r");
  if(f == NULL) {
    perror(argv[0]);
    return -1;
  }
  findlog(argv[2], directory, argv[0], secs, count);
  return 0;
}
