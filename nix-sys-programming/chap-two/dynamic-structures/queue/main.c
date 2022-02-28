#include <stdio.h>
#include <stdlib.h>

typedef struct entry {
  struct entry *next;
  int priority
} ENTRY;

void printQ(ENTRY *queue) {
  while(queue) {
    printf("[%d]->", queue->priority);
    queue = queue->next;
  }
  printf("\n");
}

void enqueue(ENTRY **queue, ENTRY *p) {
  ENTRY *q = *queue;

  // print queue before insert
  printQ(q);
  if (q == 0 || p->priority > q->priority) {
    *queue = p;
    p->next = q;
  } else {
    while (q->next && p->priority <= q->priority) {
      q = q->next;
    }
    p->next = q->next;
    q->next = p;
  }

  // print queue after insert
  printQ(q);
  printf("\n");
}

int main()
{
  // original entry
  ENTRY *orig = (ENTRY *)malloc(sizeof(ENTRY));
  orig->priority = 8;

  // first entry
  ENTRY *first = (ENTRY *)malloc(sizeof(ENTRY));
  first->priority = 5;
  enqueue(&orig, first);

  // second entry
  ENTRY *second = (ENTRY *)malloc(sizeof(ENTRY));
  second->priority = 2;
  enqueue(&orig, second);
}
