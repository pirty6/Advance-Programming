#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int maxSubArraySum(int* a, int n) {
  if(n == 0) {
    printf("The array is empty");
    exit(-1);
  }
  int i;
  int max_sum_so_far = a[0], max_ending_here = a[0];
  int max_element = -10000;

  for(i=1;i<n;i++){
    max_ending_here = max_ending_here + a[i];

    if(max_ending_here < 0){
      max_ending_here =0;
    }
    if(max_sum_so_far < max_ending_here) {
      max_sum_so_far = max_ending_here;
    }
    if(a[i] > max_element) {
      max_element = a[i];
    }
  }

  if(max_sum_so_far == 0) {
    max_sum_so_far = max_element;
  }
  return max_sum_so_far;
}

int main(int argc, char* argv[]) {
  int a[] =  {-2, -3, -4, -1, -2, -1, -5, -3};
  int n = sizeof(a)/sizeof(a[0]);
  int max_sum = maxSubArraySum(a, n);
  printf("Max: %i\n", max_sum);
}
