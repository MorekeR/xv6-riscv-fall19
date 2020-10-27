#include "kernel/types.h"
#include "user/user.h"

void main(int argc, char *argv[]) {
    char c = argv[1][0];
    if(argc != 2 || c<'0'|| c>'9') {
    fprintf(2, "usage: sleep (a non-negative number)\n");
    exit();
  }
  sleep(atoi(argv[1]));
  fprintf(2, "nothing happen for a little while.\n");
  exit();
}
