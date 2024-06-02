#pragma once
/* hkNode_i64 */
#include "../core.h"



structdef(hkNode_i64) {
    hkNode_i64 *next;
    i64 data;
};

hkNode_i64 *hknode_i64_create(i64 data) {
    hkNode_i64 *node = malloc(sizeof(hkNode_i64));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_i64_destroy(hkNode_i64 **node) {
    free(*node);
    *node = NULL;
}

