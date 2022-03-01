#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>

int main(void)
{
    int pid, r, status;
    printf("marking process %d as subreaper\n", getpid());
    r = prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
    pid = fork();
    if (pid) {
        // parent process will execute this
        printf("subreaper process\n\tpid: %d\n\tchildpid: %d\n", getpid(), pid);
        while(1) {
            pid = wait(&status);
            if (pid > 0)
                printf("\tzombie process\n\t\tpid: %d\n\t\treaper: %d\n", pid, getpid());
            else
                break;
        }
    } else {
        // child process will execute this
        printf("child process\n\tpid: %d\n\tppid: %d\n", getpid(), getppid());
        pid = fork();
        if (pid) {
            // child
            printf("\tcurrent process[START]\n\t\tpid: %d\n\t\tchildpid: %d\n", getpid(), pid);
            printf("\tcurrent process[EXIT]\n\t\tpid: %d\n\t\tppid: %d\n", getpid(), getppid());
        } else {
            // grandchild
            printf("\tgrandchild process[START]\n\t\tpid: %d\n\t\tppid: %d\n", getpid(), getppid());
            printf("\tgrandchild process[EXIT]\n\t\tpid: %d\n\t\tppid: %d\n", getpid(), getppid());
        }
    }
}
