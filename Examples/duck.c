#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct s {
  int pos;
  int flag;
}

int main(int argc, char* argv[]) {
  if(argc != 4) {
    return -1;
  }
  int students_count = atoi(argv[1]);
  int times_duck = atoi(argv[2]);
  int times = atoi(argv[3]);
  if(students_count <= 0 || times < 0 || times_duck < 0) {
    return -1;
  }

  struct s students[students_count];
  int i, count = 0, count_ducks = 0;
  for(i = 0; i < students_count; i++) {
    students[i]->pos = i;
    students[i]->flag = 0;
  }
  i = 0;
  while(count < times) {
      while(count_ducks < times_duck) {
        if(i == students_count - 1) {
          i = 0;
        }
        if(students[i]->flag == 0) {
          count_ducks++;
        }
        i++;
      }
      count++;
  }
  return students[i];
}
