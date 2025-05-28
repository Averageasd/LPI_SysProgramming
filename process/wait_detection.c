#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Main process here %d\n", getpid());
    int childPid;
    int status;
    int p = fork();
    if (p == 0)
    {
        printf("child %d doing some work\n", getpid());
        while (1)
        {
            pause();
        }
        exit(0);
    }

    childPid = waitpid(-1, &status, WUNTRACED);
    while (!WIFEXITED(status) && !WIFSIGNALED(status))
    {
        if (WIFSTOPPED(status))
        {
            printf("process %d stops.\n", childPid);
            childPid = waitpid(-1, &status, WUNTRACED);
        }
    }

    if (WIFEXITED(status))
    {
        printf("process %d exits normally.\n", childPid);
    }
    else if (WIFSIGNALED(status))
    {
        printf("process %d killed by signal %d\n", childPid, WTERMSIG(status));
    }
}