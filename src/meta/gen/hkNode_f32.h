#pragma once
/* hkNode_f32 */
#include "../core.h"



structdef(hkNode_f32) {
    hkNode_f32 *next;
    f32 data;
};

hkNode_f32 *hknode_f32_create(f32 data) {
    hkNode_f32 *node = malloc(sizeof(hkNode_f32));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_f32_destroy(hkNode_f32 **node) {
    free(*node);
    *node = NULL;
}

