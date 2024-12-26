#pragma once
/* Stack_TYPE */
#include <core.h>

#include "TYPE.h"
#include "Node_TYPE.h"

structdef(Stack_TYPE) {
    Node_TYPE *head;
    usize length;
};

Stack_TYPE *Stack_TYPE_create();
void Stack_TYPE_push(Stack_TYPE *stack, TYPE data);
Node_TYPE *Stack_TYPE_pop(Stack_TYPE *stack);
Node_TYPE *Stack_TYPE_peek(Stack_TYPE *stack);
void Stack_TYPE_destroy(Stack_TYPE **stack);
void Stack_TYPE_print(Stack_TYPE *stack);
