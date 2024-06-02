#pragma once
/* hkNode_u32 */
#include "../core.h"



structdef(hkNode_u32) {
    hkNode_u32 *next;
    u32 data;
};

hkNode_u32 *hknode_u32_create(u32 data) {
    hkNode_u32 *node = malloc(sizeof(hkNode_u32));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_u32_destroy(hkNode_u32 **node) {
    free(*node);
    *node = NULL;
}

