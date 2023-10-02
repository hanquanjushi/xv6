#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    char buf;
    int p[2];

    if (pipe(p) == -1)
    {
        fprintf(2, "Cannot create a pipe\n");
        exit(-1);
    }

    int child_pid = fork();

    if (child_pid == -1)
    {
        fprintf(2, "Cannot fork a child process\n");
        exit(-1);
    }

    if (child_pid > 0)
    {
   

        // Parent process sends a byte to the child
        write(p[1], "a", 2);
        close(p[1]);

        // Wait for the child to exit
        wait(0);

        // Parent receives a byte from the child
        read(p[0], &buf, sizeof(buf));
        printf("%d: received pong\n", getpid());
        close(p[0]);
    }
    else if (child_pid == 0)
    {
        // Child receives a byte from the parent
        read(p[0], &buf, sizeof(buf));
        printf("%d: received ping\n", getpid());

        // Child sends a byte to the parent
        write(p[1], "b", 2);

        close(p[0]);
        close(p[1]);
        exit(0);
    }

    exit(0);
}
