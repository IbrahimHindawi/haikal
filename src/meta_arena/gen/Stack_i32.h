#pragma once
/* Stack_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);
#include "Node_i32.h"

structdef(Stack_i32) {
    Node_i32 *head;
    usize length;
};

Stack_i32 *Stack_i32_create(Arena *arena);
void Stack_i32_push(Arena *arena, Stack_i32 *stack, i32 data);
Node_i32 *Stack_i32_pop(Arena *arena, Stack_i32 *stack);
Node_i32 *Stack_i32_peek(Arena *arena, Stack_i32 *stack);
void Stack_i32_destroy(Arena *arena, Stack_i32 **stack);
void Stack_i32_print(Arena *arena, Stack_i32 *stack);

