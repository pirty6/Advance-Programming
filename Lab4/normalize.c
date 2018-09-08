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

#define SIZE 	1000
#define WIDTH 	68

int values[SIZE];

int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Usage: %s [input_directory] [output_directory]\n", argv[0]);
    return -1;
  }
  DIR* dir;
  DIR* dir2;
  struct dirent* direntry;
  char filename[PATH_MAX + NAME_MAX + 1];
  FILE *fd_in;
  int time, intersections, size, i;
  float num;


  if((dir = opendir(argv[1])) == NULL || (dir2 = opendir(argv[2])) == NULL ) {
    perror(argv[0]);
    return -1;
  }

  while ((direntry = readdir(dir)) != NULL) {
    if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
      if(direntry->d_type != DT_DIR) {
        // printf("%s\n", direntry->d_name);
        sprintf(filename, "%s/%s", argv[1], direntry->d_name);
        fd_in = fopen(filename, "r");
        if (fd_in == NULL) {
          perror(argv[0]);
		      return -1;
	      }
        memset(values, 0, SIZE * sizeof(int));
        while ( fscanf(fd_in, "%i,%i", &time, &intersections) != EOF ) {
    		size = intersections * intersections;
    		for (i = 0; i < size; i++) {
    			fscanf(fd_in, ",%f", &num);
    			values[i] = (int)num;
          printf("%i ", values[i]);
    		}
    	 }
      fclose(fd_in);
      }
    }
  }
}
