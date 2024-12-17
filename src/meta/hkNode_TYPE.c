/* hkNode_TYPE */
#include <core.h>

#include "hkNode_TYPE.h"

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

// no optional types, handled at callsite.
// maybe add a macro?
TYPE *hkNode_TYPE_value(hkNode_TYPE *node) {
    // TYPE result;
    // if (node) {
    //     result = node->data;
    // } else {
    //     printf("hkNode_TYPE is NULL.\n");
    //     exit(1);
    // };
    // return result;
    // if (!node) {
    //     return node->data;
    // } else {
    //     printf("node was null!\n");
    // }
    // return NULL;
    return NULL;
}
