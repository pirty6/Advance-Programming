#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

int min=0, max=0, count=0;
long total=0;




void get_size(char *filename, char *program){
    struct stat info;
    int length;
    if(lstat(filename,&info)<0){
      perror(program);
      exit(-1);
    }

    int fd;
    if((fd=open(filename, O_RDONLY))<0){
			perror(program);
			exit(-1);
		}

    length = lseek(fd, 0, SEEK_END)+1;

    if(min>length)
      min=length;

    if(max<length)
      max=length;

    total += length;
    close(fd);
}


void info(char* name){
  printf("--------------------------------------------------------\n");
  printf("Directory: %s\n", name);
  printf("Occupied Space: %li Total count: %i\n", total, count);
  printf("Min size: %i Max Size: %i\n", min, max);
  min=INT_MAX;
  max=INT_MIN;
  total = 0;
  count = 0;
}

void list(char *directory, char *program){
  DIR* dir;
  struct dirent* direntry;
  struct stat inf;
  char filename[PATH_MAX + NAME_MAX + 1];

  if((dir = opendir(directory)) == NULL){
    perror(program);
    exit(-1);
  }

  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
      sprintf(filename,"%s/%s",directory,direntry->d_name);
      lstat(filename, &inf);
      if(S_ISDIR(inf.st_mode)){
        list(filename,program);
        info(filename);
        count = 0;
      }else{
        if(direntry->d_type != DT_DIR) {
          count++;
          get_size(filename, program);
        }
      }
    }
  }

  printf("\n");
}

// void list(char *directory, char *program){
//   DIR* dir;
//   struct dirent* direntry;
//   struct stat inf;
//   char filename[PATH_MAX + NAME_MAX + 1];
//
//   if((dir = opendir(directory)) == NULL){
//     perror(program);
// 		exit(-1);
// 	}
//
//   while((direntry = readdir(dir)) != NULL){
//     if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
//       sprintf(filename,"%s/%s",directory,direntry->d_name);
//       lstat(filename, &inf);
//       if(S_ISDIR(inf.st_mode)){
//         list(filename,program);
//         info(filename);
//         count = 0;
//       }else{
//         count++;
//         get_size(filename, program);
//       }
//     }
//   }
//
//   printf("\n");
// }

int main(int argc, char *argv[]){
  if(argc!=2){
    printf("Usage: %s [directory]\n", argv[0]);
    return -1;
  }
  char *directory;
  directory = argv[1];
  list(directory, argv[0]);
  return 0;
}
