#pragma once
/* Queue_TYPE */
#include <core.h>

#include "TYPE.h"
#include "Node_TYPE.h"

structdef(Queue_TYPE) {
    Node_TYPE *head;
    Node_TYPE *tail;
    usize length;
};

Queue_TYPE *Queue_TYPE_create();
void Queue_TYPE_enqueue(Queue_TYPE *queue, TYPE data);
Node_TYPE *Queue_TYPE_dequeue(Queue_TYPE *queue);
Node_TYPE *Queue_TYPE_peek(Queue_TYPE *queue);
void Queue_TYPE_destroy(Queue_TYPE **queue);
void Queue_TYPE_print(Queue_TYPE *queue);
