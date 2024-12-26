#pragma once
/* BiNode_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(BiNode_TYPE) {
    BiNode_TYPE *next;
    BiNode_TYPE *prev;
    TYPE data;
};

BiNode_TYPE *BiNode_TYPE_create(TYPE data);
void BiNode_TYPE_destroy(BiNode_TYPE **node);
TYPE BiNode_TYPE_value(BiNode_TYPE *node);

#define BiNode_TYPE_get(node, value) \
    if (!(node)) { \
        printf(#node \
                " was null!\n"); \
        /* debugbreak? */ \
    } else { \
        (value) = (node)->data; \
    }
