#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int pid, status;
    printf("current process\n\tpid: %d\n\tppid: %d\n", getpid(), getppid());
    pid = fork();
    if (pid) {
        // parent process will execute this
        printf("parent process\n\tpid: %d\n\tchildpid: %d\n", getpid(), pid);
        pid = wait(&status);
        printf("zombie process\n\tpid: %d\n\tstatus: 0x%04x\n", pid, status);
    } else {
        // child process will execute this
        printf("child process\n\tpid: %d\n\tppid: %d\n", getpid(), getppid());
        exit(100);
    }
}
