#pragma once
/* hkNode_i8 */
#include "../core.h"



structdef(hkNode_i8) {
    hkNode_i8 *next;
    i8 data;
};

hkNode_i8 *hknode_i8_create(i8 data) {
    hkNode_i8 *node = malloc(sizeof(hkNode_i8));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_i8_destroy(hkNode_i8 **node) {
    free(*node);
    *node = NULL;
}

