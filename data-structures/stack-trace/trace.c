#include <stdio.h>
#include <stdlib.h>

/** 
 * I recently discovered that you could actually get the stack trace of 
 * an executing process by using /proc. I was happy but you had to do it
 * by first getting the process id of the process then reading the stack
 * trace from the /proc directory.
 * To access a processes stack trace you
 *
 * `sudo cat /proc/<PID>/stack` 
 * 
 * I want to eliminate getting the PID aspect and just access the stack trace
 * by `sudo trace <process name> stack` and you should get the stack trace
 * */

void usage(char* exe) {
    printf("Usage:\n\t%s <procname> <info> \n<info>:\n\tcgroups, stat, mounts, limits\n", exe);
}

void main(int argc, char** argv) {
    // needs a proces name and what to trace.
    //
    // for now trace will support
    // cgroups, stat, stack, mounts, limits

    if (argc != 3) {
        usage(argv[0]);
        exit(2);
    }

    printf("everything cool!\n");
}
