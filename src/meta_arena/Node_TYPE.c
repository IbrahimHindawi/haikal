/* Node_TYPE */
#include <core.h>

#include "Node_TYPE.h"

Node_TYPE *Node_TYPE_create(Arena *arena, TYPE data) {
    // Node_TYPE *node = malloc(sizeof(Node_TYPE));
    Node_TYPE *node = arenaPushStruct(arena, Node_TYPE);
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void Node_TYPE_destroy(Arena *arena, Node_TYPE **node) {
    // free(*node);
    *node = NULL;
}

// no optional types, handled at callsite.
// maybe add a macro?
TYPE *Node_TYPE_value(Arena *arena, Node_TYPE *node) {
    // TYPE result;
    // if (node) {
    //     result = node->data;
    // } else {
    //     printf("Node_TYPE is NULL.\n");
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
