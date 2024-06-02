#pragma once
/* hkQueue_i64 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i64.h"

structdef(hkQueue_i64) {
    hkNode_i64 *head;
    hkNode_i64 *tail;
    usize length;
};

hkQueue_i64 *hkqueue_i64_create() {
    hkQueue_i64 *queue = malloc(sizeof(hkQueue_i64));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_i64_enqueue(hkQueue_i64 *queue, i64 data) {
    if (!queue->head) {
        queue->head = hknode_i64_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_i64_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_i64 *temp = hknode_i64_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_i64 *hkqueue_i64_dequeue(hkQueue_i64 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_i64 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_i64 *hkqueue_i64_peek(hkQueue_i64 *queue) { 
    return queue->head; 
}

void hkqueue_i64_destroy(hkQueue_i64 **queue) {
    free(*queue);
    *queue = NULL;
}

