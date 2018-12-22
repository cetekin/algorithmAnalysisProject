#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*Implementation of circular queue data structure*/
/*************************************************/


QUEUE* create_queue() {

        QUEUE* q = (QUEUE*)malloc(sizeof(QUEUE));

        if (q == NULL) {
                printf("Memory is not allocated!\n");
                exit(-6);
        }

        q->front = 0;
        q->rear = 0;
        q->cnt = 0;

        return q;

}

/*Checks whether queue is emty or not*/
int is_empty(QUEUE* q) {

        if (q->cnt == 0) {
                //printf("Queue is empty!\n");
                return 1;
        }

        return 0;

}

/*Checks whether queue is full or not*/
int is_full(QUEUE* q) {

        if (q->cnt == SIZE) {
                printf("Queue is full!\n");
                return 1;
        }
        return 0;
}

/*Enqueues given element to the queue*/
int enqueue(QUEUE* q,int item) {

        if (is_full(q)) {
                exit(-4);

        }



        q->qu[q->rear] = item;
        q->rear = (q->rear+1)%SIZE;
        q->cnt++;


        return 1;
}

/*Dequeues an element from queue*/
int dequeue(QUEUE* q) {

        int item;
        if (is_empty(q)) {
                exit(-5);
        }


        item = q->qu[q->front];
        q->front = (q->front+1)%SIZE;
        q->cnt--;
        return item;



}
