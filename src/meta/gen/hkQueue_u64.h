#pragma once
/* hkQueue_u64 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u64.h"

structdef(hkQueue_u64) {
    hkNode_u64 *head;
    hkNode_u64 *tail;
    usize length;
};

hkQueue_u64 *hkqueue_u64_create() {
    hkQueue_u64 *queue = malloc(sizeof(hkQueue_u64));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_u64_enqueue(hkQueue_u64 *queue, u64 data) {
    if (!queue->head) {
        queue->head = hknode_u64_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_u64_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_u64 *temp = hknode_u64_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_u64 *hkqueue_u64_dequeue(hkQueue_u64 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_u64 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_u64 *hkqueue_u64_peek(hkQueue_u64 *queue) { 
    return queue->head; 
}

void hkqueue_u64_destroy(hkQueue_u64 **queue) {
    free(*queue);
    *queue = NULL;
}

