#pragma once
/* hkQueue_u8 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u8.h"

structdef(hkQueue_u8) {
    hkNode_u8 *head;
    hkNode_u8 *tail;
    usize length;
};

hkQueue_u8 *hkqueue_u8_create() {
    hkQueue_u8 *queue = malloc(sizeof(hkQueue_u8));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_u8_enqueue(hkQueue_u8 *queue, u8 data) {
    if (!queue->head) {
        queue->head = hknode_u8_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_u8_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_u8 *temp = hknode_u8_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_u8 *hkqueue_u8_dequeue(hkQueue_u8 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_u8 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_u8 *hkqueue_u8_peek(hkQueue_u8 *queue) { 
    return queue->head; 
}

void hkqueue_u8_destroy(hkQueue_u8 **queue) {
    free(*queue);
    *queue = NULL;
}

