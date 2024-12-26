#include <core.h>

#include "Queue_TYPE.h"

Queue_TYPE *Queue_TYPE_create() {
    Queue_TYPE *queue = malloc(sizeof(Queue_TYPE));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void Queue_TYPE_enqueue(Queue_TYPE *queue, TYPE data) {
    if (!queue->head) {
        queue->head = Node_TYPE_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = Node_TYPE_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    queue->length += 1;
    Node_TYPE *node = Node_TYPE_create(data);
    queue->tail->next = node; 
    queue->tail = node;
}

Node_TYPE *Queue_TYPE_dequeue(Queue_TYPE *queue) {
    if (!queue->head) {
        return NULL;
    }
    queue->length -= 1;
    Node_TYPE *result = queue->head;
    queue->head = queue->head->next;
    result->next = NULL;

    if (queue->length == 0) {
        queue->tail = NULL;
    }
    return result;
}

Node_TYPE *Queue_TYPE_peek(Queue_TYPE *queue) { 
    return queue->head; 
}

void Queue_TYPE_destroy(Queue_TYPE **queue) {
    free(*queue);
    *queue = NULL;
}

void Queue_TYPE_print(Queue_TYPE *queue) { 
    Node_TYPE *iter = queue->head; 
    printf("Queue length = %llu\n", queue->length);
    while (iter) { 
        printf("queue: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    // printf("\n");
}
