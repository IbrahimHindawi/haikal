#pragma once
/* hkNode_u64 */
#include "../core.h"



structdef(hkNode_u64) {
    hkNode_u64 *next;
    u64 data;
};

hkNode_u64 *hknode_u64_create(u64 data) {
    hkNode_u64 *node = malloc(sizeof(hkNode_u64));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_u64_destroy(hkNode_u64 **node) {
    free(*node);
    *node = NULL;
}

