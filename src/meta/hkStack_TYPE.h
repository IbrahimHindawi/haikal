#pragma once
/* hkStack_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkNode_TYPE.h"

structdef(hkStack_TYPE) {
    hkNode_TYPE *head;
    usize length;
};

hkStack_TYPE *hkStack_TYPE_create();
void hkStack_TYPE_push(hkStack_TYPE *stack, TYPE data);
hkNode_TYPE *hkStack_TYPE_pop(hkStack_TYPE *stack);
hkNode_TYPE *hkStack_TYPE_peek(hkStack_TYPE *stack);
void hkStack_TYPE_destroy(hkStack_TYPE **stack);
void hkStack_TYPE_print(hkStack_TYPE *stack);
