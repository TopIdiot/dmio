/*
 * =====================================================================================
 *
 *       Filename:  queue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年02月22日 17时30分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "dmio_type.h"

typedef struct Node{
    msg data;
    struct Node *next;
} Node, *Queue;

typedef struct{
    Queue front;
    Queue rear;
} LinkQueue;
void initQueue(LinkQueue *queue)

bool isEmpty(LinkQueue *queue);

void push_queue(LinkQueue *queue, msg temp);

void pop_queue(LinkQueue *queue);

Node* front_queue(LinkQueue *queue);

void destoey_queue(LinkQueue *queue);

#endif
