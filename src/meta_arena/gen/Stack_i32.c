#include <core.h>

#include "Stack_i32.h"

Stack_i32 *Stack_i32_create(Arena *arena) {
    // Stack_i32 *stack = malloc(sizeof(Stack_i32));
    Stack_i32 *stack = arenaPushStruct(arena, Stack_i32);
    stack->head = NULL;
    stack->length = 0;
    return stack;
}

void Stack_i32_push(Arena *arena, Stack_i32 *stack, i32 data) {
    stack->length += 1;
    if (!stack->head) {
        Node_i32 *node = Node_i32_create(arena, data);
        stack->head = node;
        return;
    }
    Node_i32 *node = Node_i32_create(arena, data);
    node->next = stack->head;
    stack->head = node;
}

Node_i32 *Stack_i32_pop(Arena *arena, Stack_i32 *stack) {
    if (stack->length > 0) {
        stack->length -= 1;
    }
    if (stack->length == 0) {
        Node_i32 *head_node = stack->head;
        stack->head = NULL;
        return head_node;
    }
    Node_i32 *head_node = stack->head;
    stack->head = head_node->next;
    head_node->next = NULL;
    return head_node;
}

Node_i32 *Stack_i32_peek(Arena *arena, Stack_i32 *stack) { 
    return stack->head;
}

void Stack_i32_destroy(Arena *arena, Stack_i32 **stack) {
    // free(*stack);
    *stack = NULL;
}

void Stack_i32_print(Arena *arena, Stack_i32 *stack) { 
    Node_i32 *iter = stack->head; 
    printf("Stack length = %llu\n", stack->length);
    while (iter) { 
        printf("stack: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    // printf("\n");
}

