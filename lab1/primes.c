#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define PIPE_W 1
#define PIPE_R 0

void *child(int *p)
{

    close(p[PIPE_W]);
    int num;
    if (read(p[PIPE_R], &num, sizeof(num)) == 0)
    {
        close(p[PIPE_R]);
        exit(0);
    }
    printf("prime %d\n", num);
    int cp[2];
    pipe(cp);


    if (fork() == 0) 
    {
        child(cp);
    }
    else 
    {
        close(cp[PIPE_R]);
        int nnum;
        while (read(p[PIPE_R], &nnum, sizeof(nnum))) 
        {
            if (nnum % num != 0)
            {
                write(cp[PIPE_W], &nnum, sizeof(nnum));
            }
        }
        close(cp[PIPE_W]);
        wait(0);
    }

    exit(0);
}
int main(int argc, char const *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0) 
    {
        child(p);
    }
    else 
    {
        close(p[PIPE_R]);

        for (int i = 2; i <= 35; i++)
        {
            write(p[PIPE_W], &i, sizeof(i));
        }
        close(p[PIPE_W]);
        wait(0);
    }

    exit(0);
}
