#pragma once
/* Stack_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"
#include "Node_TYPE.h"

structdef(Stack_TYPE) {
    Node_TYPE *head;
    usize length;
};

Stack_TYPE *Stack_TYPE_create(memops_arena *arena);
void Stack_TYPE_push(memops_arena *arena, Stack_TYPE *stack, TYPE data);
Node_TYPE *Stack_TYPE_pop(memops_arena *arena, Stack_TYPE *stack);
Node_TYPE *Stack_TYPE_peek(memops_arena *arena, Stack_TYPE *stack);
void Stack_TYPE_destroy(memops_arena *arena, Stack_TYPE **stack);
void Stack_TYPE_print(memops_arena *arena, Stack_TYPE *stack);
