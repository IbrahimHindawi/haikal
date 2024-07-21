#pragma once
/* hkBiNode_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(hkBiNode_TYPE) {
    hkBiNode_TYPE *next;
    hkBiNode_TYPE *prev;
    TYPE data;
};

hkBiNode_TYPE *hkBiNode_TYPE_create(TYPE data);
void hkBiNode_TYPE_destroy(hkBiNode_TYPE **node);
TYPE hkBiNode_TYPE_value(hkBiNode_TYPE *node);

// #define HKNODE_IMPL 1
#ifdef HKBINODE_IMPL
hkBiNode_TYPE *hkBiNode_TYPE_create(TYPE data) {
    hkBiNode_TYPE *node = malloc(sizeof(hkBiNode_TYPE));
    if (node) {
        node->next = NULL;
        node->prev = NULL;
        node->data = data;
    }
    return node;
}

void hkBiNode_TYPE_destroy(hkBiNode_TYPE **node) {
    free(*node);
    *node = NULL;
}

TYPE hkBiNode_TYPE_value(hkBiNode_TYPE *node) {
    TYPE result;
    if (node) { 
        result = node->data; 
    } else {  
        printf("hkBiNode_TYPE is NULL.\n"); 
        exit(1);
    };
    return result;
}
#endif
#undef HKBINODE_IMPL
