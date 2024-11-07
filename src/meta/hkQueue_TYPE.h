#pragma once
/* hkQueue_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkNode_TYPE.h"

structdef(hkQueue_TYPE) {
    hkNode_TYPE *head;
    hkNode_TYPE *tail;
    usize length;
};

hkQueue_TYPE *hkQueue_TYPE_create();
void hkQueue_TYPE_enqueue(hkQueue_TYPE *queue, TYPE data);
hkNode_TYPE *hkQueue_TYPE_dequeue(hkQueue_TYPE *queue);
hkNode_TYPE *hkQueue_TYPE_peek(hkQueue_TYPE *queue);
void hkQueue_TYPE_destroy(hkQueue_TYPE **queue);
void hkQueue_TYPE_print(hkQueue_TYPE *queue);
