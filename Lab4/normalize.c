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

float values[SIZE][SIZE];

int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Usage: %s [input_directory] [output_directory]\n", argv[0]);
    return -1;
  }
  DIR* dir;
  DIR* dir2;
  struct dirent* direntry;
  char filename[PATH_MAX + NAME_MAX + 1];
  char filenameout[PATH_MAX + NAME_MAX + 1];
  FILE *fd_in;
  FILE *fd_out;
  int time, intersections, size, i, j=0;
  float num;
  float max = 0.0, min = 0.0;
  int count = 0;
  int aux = 0;


  if((dir = opendir(argv[1])) == NULL || (dir2 = opendir(argv[2])) == NULL ) {
    perror(argv[0]);
    return -1;
  }

  while ((direntry = readdir(dir)) != NULL) {
    if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
      if(direntry->d_type != DT_DIR) {
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
    			values[i][j] = num;
          // printf("%f", values[i][j]);
          if(values[i][j] > max) {
            max = values[i][j];
          }
          if(values[i][j] < min) {
            min = values[i][j];
          }
          aux++;
    		}
        j++;
        count = j;
    	 }
       printf("\n%f", max);
       // printf("\n%f\n", min);
       sprintf(filenameout, "%s/%s", argv[2], direntry->d_name);
       fd_out = fopen(filenameout, "w");
       if (fd_out == NULL) {
         perror(argv[0]);
         return -1;
       }
       for(j = 0; j < count; j++) {
         for(i = 0; i < size; i++) {
           values[i][j] = ((values[i][j]-min)/(max-min));
           fprintf(fd_out, "%f,",values[i][j]);
           // printf("%f ", values[i][j]);
         }
       }
       // printf("%i\n", aux);
      // fwrite(values, sizeof(float), aux, fd_out);
      fclose(fd_in);
      fclose(fd_out);
      j = 0;
      max = 0;
      min = 0;
      }
    }
  }
  return 0;
}
