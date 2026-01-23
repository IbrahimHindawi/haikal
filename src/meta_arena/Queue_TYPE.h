#pragma once
/* Queue_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"
#include "Node_TYPE.h"

structdef(Queue_TYPE) {
    Node_TYPE *head;
    Node_TYPE *tail;
    usize length;
};

Queue_TYPE *Queue_TYPE_create(memops_arena *arena);
void Queue_TYPE_enqueue(memops_arena *arena, Queue_TYPE *queue, TYPE data);
Node_TYPE *Queue_TYPE_dequeue(memops_arena *arena, Queue_TYPE *queue);
Node_TYPE *Queue_TYPE_peek(memops_arena *arena, Queue_TYPE *queue);
void Queue_TYPE_destroy(memops_arena *arena, Queue_TYPE **queue);
void Queue_TYPE_print(memops_arena *arena, Queue_TYPE *queue);
