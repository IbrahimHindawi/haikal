#pragma once
/* hkNode_u8 */
#include "../core.h"



structdef(hkNode_u8) {
    hkNode_u8 *next;
    u8 data;
};

hkNode_u8 *hknode_u8_create(u8 data) {
    hkNode_u8 *node = malloc(sizeof(hkNode_u8));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_u8_destroy(hkNode_u8 **node) {
    free(*node);
    *node = NULL;
}

