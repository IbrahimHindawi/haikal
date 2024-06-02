#pragma once
/* hkQueue_u32 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u32.h"

structdef(hkQueue_u32) {
    hkNode_u32 *head;
    hkNode_u32 *tail;
    usize length;
};

hkQueue_u32 *hkqueue_u32_create() {
    hkQueue_u32 *queue = malloc(sizeof(hkQueue_u32));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_u32_enqueue(hkQueue_u32 *queue, u32 data) {
    if (!queue->head) {
        queue->head = hknode_u32_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_u32_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_u32 *temp = hknode_u32_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_u32 *hkqueue_u32_dequeue(hkQueue_u32 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_u32 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_u32 *hkqueue_u32_peek(hkQueue_u32 *queue) { 
    return queue->head; 
}

void hkqueue_u32_destroy(hkQueue_u32 **queue) {
    free(*queue);
    *queue = NULL;
}

