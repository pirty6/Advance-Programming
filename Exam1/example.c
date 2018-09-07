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

int min=INT_MAX, max=INT_MIN, files=0;
long total=0;


void calc_size(char *filename, char *program){
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

    length = lseek(fd, 0, SEEK_END);

    if(min>length)
      min=length;

    if(max<length)
      max=length;

    total += length;

    // printf("min %i max %i total %li \n", min, max, total);

    close(fd);
}


void get_info(char* name){
  printf("--------------------------------------------------------\n");
  printf("Directory: %s\n", name);
  printf("Occupied Space: %li Total Files: %i\n", total, files);
  printf("Min size: %i Max Size: %i\n", min, max);
  min=INT_MAX;
  max=INT_MIN;
  // total = 0;
  files = 0;
}

void list(char *directory, char *program){
  DIR* dir;
  struct dirent* direntry;
  struct stat info;
  char filename[PATH_MAX + NAME_MAX + 1];

  if((dir = opendir(directory)) == NULL){
    printf("Usage: %s [directory]\n", program);
    perror(program);
		exit(-1);
	}

  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
      sprintf(filename,"%s/%s",directory,direntry->d_name);
      lstat(filename, &info);
      if(S_ISDIR(info.st_mode)){
        list(filename,program);
        get_info(filename);
        // files = 0;
      }else{
        files++;
        // printf("FILENAME %s count: %i\n", filename, files);
        calc_size(filename, program);
      }
    }
  }
}

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
