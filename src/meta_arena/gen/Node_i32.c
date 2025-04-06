/* Node_i32 */
#include <core.h>

#include "Node_i32.h"

Node_i32 *Node_i32_create(Arena *arena, i32 data) {
    // Node_i32 *node = malloc(sizeof(Node_i32));
    Node_i32 *node = arenaPushStruct(arena, Node_i32);
    if (node) {
        node->next = NULL;
        node->data = data;
    }
    return node;
}

void Node_i32_destroy(Arena *arena, Node_i32 **node) {
    // free(*node);
    *node = NULL;
}

// no optional types, handled at callsite.
// maybe add a macro?
i32 *Node_i32_value(Arena *arena, Node_i32 *node) {
    // i32 result;
    // if (node) {
    //     result = node->data;
    // } else {
    //     printf("Node_i32 is NULL.\n");
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

