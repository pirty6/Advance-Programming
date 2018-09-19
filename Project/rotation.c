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

void changeName(char* name, char* directory, char* program) {
  FILE *fd_out;
  FILE *fd_in;
  char ch;
  char filename[PATH_MAX + NAME_MAX + 1];
  char filenameoriginal[PATH_MAX + NAME_MAX + 1];
  sprintf(filenameoriginal, "%s/%s", directory, name);
  char* extension = NULL;
  memset(filename, 0, sizeof(filename));
  extension = strrchr(name, '.');
  char* ex;
  int i, count = 1, flag = 0;
  int extensionsize = strlen(extension);
  for(i = 1; i < extensionsize; i++) {
    if(!isdigit(extension[i])) {
      flag = 1;
    }
  }
  if(flag == 0) {
    memmove(&extension[0], &extension[1], strlen(extension) - 0);
    count = atoi(extension);
    count++;
  }
  if (extension != NULL) {
    *extension = '\0';
  }
  if(directory[0] == '.') {
    sprintf(filename, "%s%s.%i", directory, name, count);
  } else {
    sprintf(filename, "%s/%s.%i", directory, name, count);
  }
  fd_in = fopen(filenameoriginal, "r");
  if (fd_in == NULL) {
    perror(program);
    exit (-1);
  }
  fd_out = fopen(filename, "w");
  while( ( ch = fgetc(fd_in) ) != EOF ) {
    fputc(ch, fd_out);
  }
  fclose(fd_in);
  fclose(fd_out);
}

void sleepTime(int secs) {
  int retTime = time(0) + secs;
  while (time(0) < retTime);
}

void findlog(char* name, char* directory, char* program, int secs, int count) {
  DIR* dir;
  struct dirent* direntry;
  char filename[PATH_MAX + NAME_MAX + 1];
  char* files;
  char* extension;
  char* fileOrder;
  int i = 0, j, k, flag = 0, aux, size;

  while (count > 0) {
    sleepTime(secs);
    if ((dir = opendir(directory)) == NULL) {
      perror(program);
      exit(-1);
    }
    while ((direntry = readdir(dir)) != NULL) {
      if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
        if (strstr(direntry->d_name, name)) {
          changeName(direntry->d_name, directory, program);
        }
      }
    }
    count--;
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
  findlog(argv[2], directory, argv[0], secs, count);
  return 0;
}
