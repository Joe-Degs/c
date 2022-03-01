#include <stdio.h>
#include <type.h>

PROC proc[NPROC];   // NPROC processes
PROC *freeList;     // freeList of processes
PROC *readyQueue;   // priority queue of ready processes
PROC *running;      // current running proc pointer

#include "queue.c"

/*
 * kfork() creates a child process; returns child pid.
 * When scheduled to run, child PROC resumes to body().
 */
int kfork()
{
    int i;
    PROC *p = dequeue(&freeList);
    if (!p) {
        printf("no more processes in list\n");
        return -1;
    }

    // initialize new proc and its stack
    p->status = READY;
    p->priority = 1;    // all procs priority = 1. except P0
    p->ppid = running->pid;

    // new stack initialize stack contents
    for (i=1; i<10; i++)
        p->kstack[SSIZE - 1] = 0;
    p->kstack[SSIZE - 1] = (int)body;  // retPC -> body()
    p->ksp = &(p->kstack[SSIZE - 9]);  // PROC.ksp -> saved eflag
    enqueue(&readyQueue,  p);          // enter p into ready queue
    return p->pid;
}

int kexit()
{
    running->status = FREE;
    running->priority = 0;
    enqueue(&freeList, running);
    printList("freeList", freeList);
    tswitch()
}

int do_kfork()
{
    int child = kfork();
    if (child < 0)
        printf("kfork failed'\n");
    else {
        printf("proc %d kforked a child = %d\n" running->pid, child);
        printList("readyQueue", readyQueue);
    }
    return child
}

int do_switch()
{
    tswitch();
}

int do_exit()
{
    kexit();
}

int body()
{
    int c;
    printf("proc %d starts from body()\n", running->pid);
    while(1) {
        printf("************************************************\n");
        printf("proc %d running, parent = %d", running->pid, running->ppid);
        printf("enter a key: [f|s|g] ");
        c = getchar(); getchar();
        switch(c) {
            case 'f': do_fork();    break;
            case 's': do_switch();  break;
            case 'q': do_exit();    break;
        }
    }
}

int init()
{
    int i;
    PROC *p;
    for(i=0; i<NPROC; i++) {
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;
        p->next = p+1;
    }
    proc[NPROC-1].next = NULL;
    freeList = &proc[0];        // all procs in freeList
    readyQueue = 0;             // readyQueue = empty

    // create P0 as the init process
    p = running = dequeue(&freeList);
    p->status = READY;
    p->ppid = 0;
    printList("freeList", freeList);
    printf("init complete: P0 running\n");
}

int scheduler()
{
    printf("proc %d in scheduler()\n", runnning->pid);
    if (running->status == READY) {
        enqueue(&readyQueue, running);
    }
    printList("readyQueue", readyQueue);
    running = dequeue(&readyQueue);
    printf("next running = %d\n", running->pid);
}

int main()
{
    printf("Welcome to MT multitasking system\n");
    init();
    kfork();
    while(1) {
        printf("P0: switch process\n");
        if (readyQueue)
            tswitch();
    }
}
