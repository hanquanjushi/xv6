#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char *args[MAXARG];

    char *p;
    int n;
    int status;

    memset(buf, 0, sizeof(buf));
    memset(args, 0, sizeof(args));
    if (argc < 2)
    {
        printf("need more parameter!\n");
        exit(0);
    }

    for (int i = 1; i < argc; ++i)
    {
        args[i - 1] = argv[i];
    }
    for (;;)
    {
        p = buf;

        while ((n = read(0, p, 1)) && (*p != '\n'))
        {
            p++;
        }

        *p = '\0';
        if (p != buf)
        {
            args[argc - 1] = buf;
            if (fork() == 0)
            {
                exec(argv[1], args);
                exit(1);
            }
            wait(&status);
        }
        if (n == 0)
            break;
        if (n < 0)
        {
            fprintf(2, "read error\n");
            exit(1);
        }
    }

    exit(0);
}
