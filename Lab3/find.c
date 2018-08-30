/*----------------------------------------------------------------

*

* Programación avanzada: Archivos y directorios

* Fecha: 30-Aug-2018

* Autor: A01206747 Mariana Pérez García

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void info(char* name, char* path, char* file) {
  struct stat attr;
  char time[50];
  stat(file, &attr);

  printf("\n");
  printf("Name: %s\n", name);
  printf("Path: %s\n", path);
  strftime(time, 50, "%a %b %d %H:%M:%S %Y", localtime(&attr.st_atime));
  printf("Last access: %s\n", time);
  strftime(time, 50, "%a %b %d %H:%M:%S %Y", localtime(&attr.st_mtime));
  printf("Last modification: %s\n", time);
  printf("\n");

}

void list(char* name, char* directory, int recursive, char* program) {
  DIR* dir;
  struct dirent* direntry;
  char filename[PATH_MAX + NAME_MAX + 1];

  if ((dir = opendir(directory)) == NULL) {
    perror(program);
    exit(-1);
  }
  while ((direntry = readdir(dir)) != NULL) {
    if (strcmp(direntry->d_name, ".") != 0 &&
    strcmp(direntry->d_name, "..") != 0) {
      if(recursive == 0) {
        if(strstr(direntry->d_name, name)) {
          sprintf(filename, "%s/%s", directory, direntry->d_name);
          info(direntry->d_name, directory, filename);
        }
      } else {
        if(direntry->d_type == DT_DIR) {
          char path[1024];
          snprintf(path, sizeof(path), "%s/%s", directory, direntry->d_name);
          if(strstr(direntry->d_name, name)) {
            info(direntry->d_name, directory, path);
          }
          list(name, path, recursive, program);
        } else {
          if(strstr(direntry->d_name, name)) {
            sprintf(filename, "%s/%s", directory, direntry->d_name);
            info(direntry->d_name, directory, filename);
          }
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  char dir_name[NAME_MAX + 1];
  char *directory;
  int recursive = 0;

  if (argc > 4 || argc < 3) {
    printf("usage: %s str directory [-r]\n", argv[0]);
    return -1;
  }

  if (argc == 4) {
    if (strcmp(argv[3], "-r") != 0) {
      printf("usage: %s str directory [-r]\n", argv[0]);
      return -1;
    }
    recursive = 1;
  }
  getcwd(dir_name, NAME_MAX);
	directory = dir_name;
  directory = argv[2];

  list(argv[1], directory, recursive, argv[0]);
  return 0;
}
