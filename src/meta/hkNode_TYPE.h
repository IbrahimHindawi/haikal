#pragma once
/* hkNode_TYPE */
#include "../core.h"

#include "TYPE.h"

structdef(hkNode_TYPE) {
    hkNode_TYPE *next;
    TYPE data;
};

hkNode_TYPE *hknode_TYPE_create(TYPE data) {
    hkNode_TYPE *node = malloc(sizeof(hkNode_TYPE));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hknode_TYPE_destroy(hkNode_TYPE **node) {
    free(*node);
    *node = NULL;
}
