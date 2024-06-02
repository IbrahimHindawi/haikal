#pragma once
/* hkNode_f64 */
#include "../core.h"



structdef(hkNode_f64) {
    hkNode_f64 *next;
    f64 data;
};

hkNode_f64 *hknode_f64_create(f64 data) {
    hkNode_f64 *node = malloc(sizeof(hkNode_f64));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_f64_destroy(hkNode_f64 **node) {
    free(*node);
    *node = NULL;
}

