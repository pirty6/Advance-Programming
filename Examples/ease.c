#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
  int arr[] = {2, 2, 0, 4, 0, 8};
  int i, length = sizeof(arr)/sizeof(arr[0]), zero = 0;
  // printf("%i", length);
  for(i = 0; i < length-1; i++) {
    if(arr[i] != 0 && arr[i+1] != 0) {
      arr[i] *= 2;
      arr[i+1] = 0;
    }
    if(arr[i] == 0) {
      zero++;
    }
    // printf("%i\n", arr[i]);
  }
  if(arr[length-1] == 0) {
    zero++;
  }
  int count = 0;
  int* a = malloc(length * sizeof(int));
  for(i = 0; i < length; i++) {
    if(arr[i] != 0) {
      a[count] = arr[i];
      count++;
    }
  }
  for(i = count; i < length; i++) {
    a[i] = 0;
  }
  for(i = 0; i < length; i++) {
    printf("%i\n", a[i]);
  }
  return 0;
}
