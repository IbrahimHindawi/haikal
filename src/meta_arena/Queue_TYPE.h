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

Queue_TYPE *Queue_TYPE_create(Arena *arena);
void Queue_TYPE_enqueue(Arena *arena, Queue_TYPE *queue, TYPE data);
Node_TYPE *Queue_TYPE_dequeue(Arena *arena, Queue_TYPE *queue);
Node_TYPE *Queue_TYPE_peek(Arena *arena, Queue_TYPE *queue);
void Queue_TYPE_destroy(Arena *arena, Queue_TYPE **queue);
void Queue_TYPE_print(Arena *arena, Queue_TYPE *queue);
