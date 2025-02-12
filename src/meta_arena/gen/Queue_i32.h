#pragma once
/* Queue_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);
#include "Node_i32.h"

structdef(Queue_i32) {
    Node_i32 *head;
    Node_i32 *tail;
    usize length;
};

Queue_i32 *Queue_i32_create(Arena *arena);
void Queue_i32_enqueue(Arena *arena, Queue_i32 *queue, i32 data);
Node_i32 *Queue_i32_dequeue(Arena *arena, Queue_i32 *queue);
Node_i32 *Queue_i32_peek(Arena *arena, Queue_i32 *queue);
void Queue_i32_destroy(Arena *arena, Queue_i32 **queue);
void Queue_i32_print(Arena *arena, Queue_i32 *queue);

