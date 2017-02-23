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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mpi.h"

typedef struct{
    int rank;
    int nx;
    int ny;
    MPI_Win nwin;
}msg;
typedef struct Node{
    msg data;
    struct Node *next;
} Node, *Queue;
typedef struct{
    Queue front;
    Queue rear;
} LinkQueue;


void init_queue(LinkQueue *queue);

bool is_empty_queue(LinkQueue queue);

void push_queue(LinkQueue *queue, msg temp);

void pop_queue(LinkQueue *queue);

msg front_queue(LinkQueue *queue);

void destoey_queue(LinkQueue *queue);

#endif
