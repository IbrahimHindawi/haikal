#pragma once
/* hkQueue_f32 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_f32.h"

structdef(hkQueue_f32) {
    hkNode_f32 *head;
    hkNode_f32 *tail;
    usize length;
};

hkQueue_f32 *hkqueue_f32_create() {
    hkQueue_f32 *queue = malloc(sizeof(hkQueue_f32));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void hkqueue_f32_enqueue(hkQueue_f32 *queue, f32 data) {
    if (!queue->head) {
        queue->head = hknode_f32_create(data);
        queue->length += 1;
        return;
    }
    if (!queue->tail) {
        queue->tail = hknode_f32_create(data);
        queue->head->next = queue->tail;
        queue->length += 1;
        return;
    }
    hkNode_f32 *temp = hknode_f32_create(data);
    queue->tail->next = temp; 
    queue->tail = temp;
    queue->length += 1;
}

hkNode_f32 *hkqueue_f32_dequeue(hkQueue_f32 *queue) {
    if (!queue->head) {
        return NULL;
    }
    hkNode_f32 *temp_head = queue->head;
    queue->head = queue->head->next;
    queue->length -= 1;
    return temp_head;
}

hkNode_f32 *hkqueue_f32_peek(hkQueue_f32 *queue) { 
    return queue->head; 
}

void hkqueue_f32_destroy(hkQueue_f32 **queue) {
    free(*queue);
    *queue = NULL;
}

