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

Stack_TYPE *Stack_TYPE_create(Arena *arena);
void Stack_TYPE_push(Arena *arena, Stack_TYPE *stack, TYPE data);
Node_TYPE *Stack_TYPE_pop(Arena *arena, Stack_TYPE *stack);
Node_TYPE *Stack_TYPE_peek(Arena *arena, Stack_TYPE *stack);
void Stack_TYPE_destroy(Arena *arena, Stack_TYPE **stack);
void Stack_TYPE_print(Arena *arena, Stack_TYPE *stack);
