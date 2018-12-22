#ifndef __queue_h__
#define __queue_h__

#define SIZE 100

typedef struct Queue {

        int qu[SIZE];
        int front,rear;
        int cnt;

}QUEUE;

QUEUE* create_queue();
int is_empty(QUEUE*);
int is_full(QUEUE*);
int enqueue(QUEUE*,int);
int dequeue(QUEUE*);


#endif
