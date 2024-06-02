#pragma once
/* hkQueue_i16 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i16.h"

structdef(hkQueue_i16) {
    hkNode_i16 *head;
    hkNode_i16 *tail;
    usize length;
};

hkQueue_i16 *hkqueue_i16_create() {
    hkQueue_i16 *queue = malloc(sizeof(hkQueue_i16));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_i16_enqueue(hkQueue_i16 *queue, i16 data) {
    if (!queue->head) {
        queue->head = hknode_i16_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_i16_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_i16 *temp = hknode_i16_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_i16 *hkqueue_i16_dequeue(hkQueue_i16 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_i16 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_i16 *hkqueue_i16_peek(hkQueue_i16 *queue) { 
    return queue->head; 
}

void hkqueue_i16_destroy(hkQueue_i16 **queue) {
    free(*queue);
    *queue = NULL;
}

