#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "queue.h"
#include "mpi.h"
void init_queue(LinkQueue *queue)
{
    queue->front = queue->rear = (Queue)malloc(sizeof(Node));
    if (NULL == queue->front) {
        exit(0);
    }

    queue->front->next = NULL;
}
bool is_empty_queue(LinkQueue queue)
{
    return queue.rear == queue.front ? true : false;
}
void push_queue(LinkQueue *queue, msg temp)
{
    Queue q = (Queue)malloc(sizeof(Node));

    if (NULL == q) {
        exit(0);
    }
    q->data = temp;
    q->next = NULL;
    queue->rear->next = q;
    queue->rear = q;
}
void pop_queue(LinkQueue *queue)
{
    Queue q = NULL;

    if (!is_empty_queue(*queue)) {
        q = queue->front->next;
        queue->front->next = q->next;
        if (queue->rear == q) {
            queue->rear = queue->front;
        }

        free(q);
    }
}

msg front_queue(LinkQueue *queue)
{
    msg q ;

    if(!is_empty_queue(*queue))
    {
        Node *t;
        t = queue->front->next;
        q.rank = t->data.rank;
        q.ncid = t->data.ncid;
        q.varid = t->data.varid;
        q.nx = t->data.nx;
        q.ny = t->data.ny;
        q.nwin = t->data.nwin;
    }
    return q;
}

void destory_queue(LinkQueue *queue)
{
    while (queue->front != NULL) {
        queue->rear = queue->front->next;
        free(queue->front);
        queue->front = queue->rear;
    }
}
