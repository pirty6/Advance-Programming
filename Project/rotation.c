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

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: %s working_directory log_filename number_of_files timeout\n", argv[0]);
    return -1;
  }
  DIR* dir;
  struct dirent* direntry;
  char filename[PATH_MAX + NAME_MAX + 1];

  if((dir = opendir(argv[1])) == NULL) {
    perror(argv[0]);
    return -1;
  }

  printf("%s\n", argv[2]);

  FILE *fd;
  int aux = 0;
  while ((direntry = readdir(dir)) != NULL) {
    if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
      if (strstr(direntry->d_name, argv[2])) {
        aux = 1;
        printf("Found!\n");
      }
    }
  }

  if(aux == 0) {
    printf("File not found\n");
  }

  return 0;
}
