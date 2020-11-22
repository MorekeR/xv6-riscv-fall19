#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[])
{
    int numbers[100], cnt = 0, i;
    int fd[2];
    for (i = 2; i <= 35; i++)
    {
        numbers[cnt++] = i;
    }
    while (cnt > 0)
    {
        pipe(fd);
        if (fork() == 0)
        {
            int prime;
            int this_prime = 0;
            close(fd[1]);
            cnt = -1;
            while (read(fd[0], &prime, sizeof(prime)) != 0)
            {
                if (cnt == -1)
                {
                    this_prime = prime;
                    cnt = 0;
                }
                else
                {
                    if (prime % this_prime != 0)
                        numbers[cnt++] = prime;
                }
            }
            printf("prime %d\n",this_prime);
            close(fd[0]);
        }
        else
        {
            close(fd[0]);
            for (i = 0; i < cnt; i++)
            {
                write(fd[1], &numbers[i], sizeof(numbers[0]));
            }
            close(fd[1]);
            wait();
            break;
        }
    }
    exit();
}
