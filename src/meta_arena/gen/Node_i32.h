#pragma once
/* Node_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);

structdef(Node_i32) {
    Node_i32 *next;
    i32 data;
};

Node_i32 *Node_i32_create(Arena *arena, i32 data);
void Node_i32_destroy(Arena *arena, Node_i32 **node);
i32 *Node_i32_value(Arena *arena, Node_i32 *node);

#define Node_i32_get(node, value) \
    if (!(node)) { \
        printf(#node \
                " was null!\n"); \
        /* debugbreak? */ \
    } else { \
        (value) = (node)->data; \
    }

