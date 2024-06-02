#pragma once
/* hkQueue_i32 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i32.h"

structdef(hkQueue_i32) {
    hkNode_i32 *head;
    hkNode_i32 *tail;
    usize length;
};

hkQueue_i32 *hkqueue_i32_create() {
    hkQueue_i32 *queue = malloc(sizeof(hkQueue_i32));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_i32_enqueue(hkQueue_i32 *queue, i32 data) {
    if (!queue->head) {
        queue->head = hknode_i32_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_i32_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_i32 *temp = hknode_i32_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_i32 *hkqueue_i32_dequeue(hkQueue_i32 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_i32 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_i32 *hkqueue_i32_peek(hkQueue_i32 *queue) { 
    return queue->head; 
}

void hkqueue_i32_destroy(hkQueue_i32 **queue) {
    free(*queue);
    *queue = NULL;
}

