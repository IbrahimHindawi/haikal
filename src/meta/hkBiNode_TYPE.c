#include <core.h>

#include "hkBiNode_TYPE.h"

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
