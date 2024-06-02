#pragma once
/* hkQueue_f64 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_f64.h"

structdef(hkQueue_f64) {
    hkNode_f64 *head;
    hkNode_f64 *tail;
    usize length;
};

hkQueue_f64 *hkqueue_f64_create() {
    hkQueue_f64 *queue = malloc(sizeof(hkQueue_f64));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_f64_enqueue(hkQueue_f64 *queue, f64 data) {
    if (!queue->head) {
        queue->head = hknode_f64_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_f64_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_f64 *temp = hknode_f64_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_f64 *hkqueue_f64_dequeue(hkQueue_f64 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_f64 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_f64 *hkqueue_f64_peek(hkQueue_f64 *queue) { 
    return queue->head; 
}

void hkqueue_f64_destroy(hkQueue_f64 **queue) {
    free(*queue);
    *queue = NULL;
}

