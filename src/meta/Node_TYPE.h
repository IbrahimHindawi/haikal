#pragma once
/* Node_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(Node_TYPE) {
    Node_TYPE *next;
    TYPE data;
};

Node_TYPE *Node_TYPE_create(TYPE data);
void Node_TYPE_destroy(Node_TYPE **node);
TYPE *Node_TYPE_value(Node_TYPE *node);

#define Node_TYPE_get(node, value) \
    if (!(node)) { \
        printf(#node \
                " was null!\n"); \
        /* debugbreak? */ \
    } else { \
        (value) = (node)->data; \
    }
