#pragma once
/* hkQueue_i8 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i8.h"

structdef(hkQueue_i8) {
    hkNode_i8 *head;
    hkNode_i8 *tail;
    usize length;
};

hkQueue_i8 *hkqueue_i8_create() {
    hkQueue_i8 *queue = malloc(sizeof(hkQueue_i8));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_i8_enqueue(hkQueue_i8 *queue, i8 data) {
    if (!queue->head) {
        queue->head = hknode_i8_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_i8_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_i8 *temp = hknode_i8_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_i8 *hkqueue_i8_dequeue(hkQueue_i8 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_i8 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_i8 *hkqueue_i8_peek(hkQueue_i8 *queue) { 
    return queue->head; 
}

void hkqueue_i8_destroy(hkQueue_i8 **queue) {
    free(*queue);
    *queue = NULL;
}

