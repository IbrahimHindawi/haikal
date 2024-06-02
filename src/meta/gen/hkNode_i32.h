#pragma once
/* hkNode_i32 */
#include "../core.h"



structdef(hkNode_i32) {
    hkNode_i32 *next;
    i32 data;
};

hkNode_i32 *hknode_i32_create(i32 data) {
    hkNode_i32 *node = malloc(sizeof(hkNode_i32));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_i32_destroy(hkNode_i32 **node) {
    free(*node);
    *node = NULL;
}

