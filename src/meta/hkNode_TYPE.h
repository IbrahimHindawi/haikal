#pragma once
/* hkNode_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(hkNode_TYPE) {
    hkNode_TYPE *next;
    TYPE data;
};

hkNode_TYPE *hkNode_TYPE_create(TYPE data);
void hkNode_TYPE_destroy(hkNode_TYPE **node);

#ifdef HKNODE_IMPL
hkNode_TYPE *hkNode_TYPE_create(TYPE data) {
    hkNode_TYPE *node = malloc(sizeof(hkNode_TYPE));
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void hkNode_TYPE_destroy(hkNode_TYPE **node) {
    free(*node);
    *node = NULL;
}
#endif
#undef HKNODE_IMPL
