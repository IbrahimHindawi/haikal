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
TYPE *hkNode_TYPE_value(hkNode_TYPE *node);

#define hkNode_TYPE_get(node, value) \
    if (!(node)) { \
        printf(#node \
                " was null!\n"); \
        /* debugbreak? */ \
    } else { \
        (value) = (node)->data; \
    }
