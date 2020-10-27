
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[])
{
  int numbers[100], cnt = 0, i;
  int fd[2];
  for (i = 2; i <= 35; i++) {
    numbers[cnt++] = i;
  }
  // 注意fork是在这个循环内进行的
  while (cnt > 0) {
    pipe(fd);
    if (fork() == 0) {
      int prime, this_prime = 0;
      // 关闭写的一端
      close(fd[1]);
      cnt = -1;
      // 读的时候，如果父亲还没写，就会block
      while (read(fd[0], &prime, sizeof(prime)) != 0) {
          // 设置当前进程代表的素数，然后筛掉能被当前素数整除的数
        if (cnt == -1) {
          this_prime = prime;
          cnt = 0;
        } else {
            // 把筛出来的接着放在number数组里？不对，这里cnt是重新从0开始计数的
          if (prime % this_prime != 0) numbers[cnt++] = prime;
        }
      }
      // printf("pid %d ,prime %d\n",getpid(),this_prime);
        printf("prime %d\n",this_prime);
      // 关闭读
      close(fd[0]);
      // WARNING 注意！这里子进程并没有结束！子进程接下来继续执行while循环（cnt>0
      // 然后接着fork，注意此时子进程的子进程会获得和子进程一样的cnt和numbers
      // 也就是筛过的，而不是原始的
    } else {
        // 父进程里
      close(fd[0]);
      for (i = 0; i < cnt; i++) {
        write(fd[1], &numbers[i], sizeof(numbers[0]));
      }
      close(fd[1]);
      wait();
      // 这个break，让父进程直接退出循环，从而结束了
      // 即父进程只是起了往第一个子进程传原始数据的作用
 
      break;
    }
  }
  exit();
}