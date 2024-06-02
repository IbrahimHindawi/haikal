#pragma once
/* hkNode_u16 */
#include "../core.h"



structdef(hkNode_u16) {
    hkNode_u16 *next;
    u16 data;
};

hkNode_u16 *hknode_u16_create(u16 data) {
    hkNode_u16 *node = malloc(sizeof(hkNode_u16));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_u16_destroy(hkNode_u16 **node) {
    free(*node);
    *node = NULL;
}

