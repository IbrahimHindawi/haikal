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
