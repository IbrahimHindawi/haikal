#pragma once
/* hkQueue_u16 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u16.h"

structdef(hkQueue_u16) {
    hkNode_u16 *head;
    hkNode_u16 *tail;
    usize length;
};

hkQueue_u16 *hkqueue_u16_create() {
    hkQueue_u16 *queue = malloc(sizeof(hkQueue_u16));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_u16_enqueue(hkQueue_u16 *queue, u16 data) {
    if (!queue->head) {
        queue->head = hknode_u16_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_u16_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_u16 *temp = hknode_u16_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_u16 *hkqueue_u16_dequeue(hkQueue_u16 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_u16 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_u16 *hkqueue_u16_peek(hkQueue_u16 *queue) { 
    return queue->head; 
}

void hkqueue_u16_destroy(hkQueue_u16 **queue) {
    free(*queue);
    *queue = NULL;
}

