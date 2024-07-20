#pragma once
/* hkQueue_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkNode_TYPE.h"

structdef(hkQueue_TYPE) {
    hkNode_TYPE *head;
    hkNode_TYPE *tail;
    usize length;
};

hkQueue_TYPE *hkQueue_TYPE_create();
void hkQueue_TYPE_enqueue(hkQueue_TYPE *queue, TYPE data);
hkNode_TYPE *hkQueue_TYPE_dequeue(hkQueue_TYPE *queue);
hkNode_TYPE *hkQueue_TYPE_peek(hkQueue_TYPE *queue);
void hkQueue_TYPE_destroy(hkQueue_TYPE **queue);
void hkQueue_TYPE_print(hkQueue_TYPE *queue);

// #define HKQUEUE_IMPL 1
#ifdef HKQUEUE_IMPL

hkQueue_TYPE *hkQueue_TYPE_create() {
    hkQueue_TYPE *queue = malloc(sizeof(hkQueue_TYPE));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkQueue_TYPE_enqueue(hkQueue_TYPE *queue, TYPE data) {
    if (!queue->head) {
        queue->head = hkNode_TYPE_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hkNode_TYPE_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    queue->length += 1;
    hkNode_TYPE *node = hkNode_TYPE_create(data);
    queue->tail->next = node; 
    queue->tail = node;
}

hkNode_TYPE *hkQueue_TYPE_dequeue(hkQueue_TYPE *queue) {
    if (!queue->head) {
        return NULL;
    }
    queue->length -= 1;
    hkNode_TYPE *result = queue->head;
    queue->head = queue->head->next;
    result->next = NULL;
    return result;
}

hkNode_TYPE *hkQueue_TYPE_peek(hkQueue_TYPE *queue) { 
    return queue->head; 
}

void hkQueue_TYPE_destroy(hkQueue_TYPE **queue) {
    free(*queue);
    *queue = NULL;
}

void hkQueue_TYPE_print(hkQueue_TYPE *queue) { 
    hkNode_TYPE *iter = queue->head; 
    printf("Queue length = %d\n", queue->length);
    while (iter) { 
        printf("queue: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    // printf("\n");
}

#endif
