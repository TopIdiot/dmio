#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "queue.h"
#include "mpi.h"
void initQueue(LinkQueue *queue)
{
    queue->front = queue->rear = (Queue)malloc(sizeof(Node));
    if (NULL == queue->front) {
        exit(0);
    }

    queue->front->next = NULL;
}
bool is_empty(LinkQueue queue)
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

    if (!is_empty(*queue)) {
        q = queue->front->next;
        queue->front->next = q->next;
        //这句很关键，不能丢
        if (queue->rear == q) {
            queue->rear = queue->front;
        }

        free(q);
    }
}

Node* front_queue(LinkQueue *queue)
{
    Node* q = NULL;

    if(!is_empty(*queue))
    {
        q = queue->front->next;
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
