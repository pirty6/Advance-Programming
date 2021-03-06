encryption.c                                                                                        000664  001750  001750  00000003736 13340015435 014507  0                                                                                                    ustar 00mariana                         mariana                         000000  000000                                                                                                                                                                         /*----------------------------------------------------------------

*

* Programación avanzada: Apuntadores y arreglos

* Fecha: 24-Agust-2018

* Autor: A01206747 Mariana Pérez García

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

void rotate(char *c) {
  int first_item = c[0];
  for(int k=0; k<128; k++) {
    c[k] = c[k+1];
  }
  c[127]=first_item;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("usage: number origin destination\n");
    return -1;
  }
  int i;
  if(argv[1][0] == '-') {
    printf("encryption: the first parameter must be a positive integer number. \n");
    return -1;
  }
  for(i = 0; i < strlen(argv[1]); i++) {
    if(!isdigit(argv[1][i])) {
      printf("encryption: the first parameter must be a positive integer number. \n");
      return -1;
    }
  }
  int fd_out;
  if(access(argv[2], F_OK)) {
      perror(argv[0]);
      return -1;
  }

  FILE *fd_in;
  char *buffer;
  long len;
  int j;

  fd_in = fopen(argv[2], "r"); //open file in binary mode
  fseek(fd_in, 0, SEEK_END);    //jump to the end of the file
  len = ftell(fd_in);       //get the current byte offset in the file
  buffer = malloc(len);
  fseek(fd_in, 0, SEEK_SET);
  fread(buffer, 1, len,fd_in);
  fclose(fd_in);
  if(len < 128) {
    printf("Done\n");
    return 0;
  }

  if((fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0) {
    perror(argv[0]);
    return -1;
  }
  // for(int i = 0; i < 128; i++) {
  //   printf("%i", buffer[i]);
  // }
  int times = strtol(argv[1], NULL, 10);
  for(int i = 0; i < times; i++) {
    rotate(buffer);
  }
  // for(int i = 0; i < len; i++) {
  //   printf("%i", buffer[i]);
  // }
  write(fd_out, buffer, len);

  // for(int i = 0; i < len; i++) {
  //   write(fd_out, buffer[i], len);
  // }
  printf("Done\n");
  close(fd_out);



  return 0;
}
