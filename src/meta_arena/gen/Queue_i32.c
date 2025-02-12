#include <core.h>

#include "Queue_i32.h"

Queue_i32 *Queue_i32_create(Arena *arena) {
    // Queue_i32 *queue = malloc(sizeof(Queue_i32));
    Queue_i32 *queue = arenaPushStruct(arena, Queue_i32);
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void Queue_i32_enqueue(Arena *arena, Queue_i32 *queue, i32 data) {
    if (!queue->head) {
        queue->head = Node_i32_create(arena, data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = Node_i32_create(arena, data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    queue->length += 1;
    Node_i32 *node = Node_i32_create(arena, data);
    queue->tail->next = node; 
    queue->tail = node;
}

Node_i32 *Queue_i32_dequeue(Arena *arena, Queue_i32 *queue) {
    if (!queue->head) {
        return NULL;
    }
    queue->length -= 1;
    Node_i32 *result = queue->head;
    queue->head = queue->head->next;
    result->next = NULL;

    if (queue->length == 0) {
        queue->tail = NULL;
    }
    return result;
}

Node_i32 *Queue_i32_peek(Arena *arena, Queue_i32 *queue) { 
    return queue->head; 
}

void Queue_i32_destroy(Arena *arena, Queue_i32 **queue) {
    // free(*queue);
    *queue = NULL;
}

void Queue_i32_print(Arena *arena, Queue_i32 *queue) { 
    Node_i32 *iter = queue->head; 
    printf("Queue length = %llu\n", queue->length);
    while (iter) { 
        printf("queue: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    // printf("\n");
}

