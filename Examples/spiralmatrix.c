#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Usage: %s number\n", argv[0]);
    return -1;
  }

  int num = atoi(argv[1]);
  if(num < 1) {
    printf("Number must be a positive integer\n");
    return -1;
  }

  int j, k;
  int** matrix = malloc(num * sizeof(int));
  for(j = 0; j < num; j++) {
    matrix[j] = malloc(num*sizeof(int));
  }

  int i = 0, right = 1, x = 0, y = 0, down = 0, left = 0, up = 0, max_y = num, max_x = num;
  while(i < num*num) {
    if(right == 1) {
      for(x; x < max_x; x++) {
        matrix[y][x] = ++i;
      }
      right = 0;
      down = 1;
      y++;
      x--;
    }
    if(down == 1) {
      for(y; y < max_y; y++) {
        matrix[y][x] = ++i;
      }
      left = 1;
      down = 0;
      x--;
      y--;
    }
    if(left == 1) {
      for(x; x > num - max_x-1; x--) {
        matrix[y][x] = ++i;
      }
      up = 1;
      left = 0;
      y--;
      max_x--;
      x++;
    }
    if(up == 1) {
      printf("%i\n", x);
      for(y; y > num - max_y; y--) {
        matrix[y][x] = ++i;
      }
      up = 0;
      right = 1;
      max_y--;
      x++;
      y++;
    }
  }

  for(j = 0; j < num; j++) {
    for(k = 0; k < num; k++) {
      printf("%i", matrix[j][k]);
    }
    printf("\n");
  }
}
