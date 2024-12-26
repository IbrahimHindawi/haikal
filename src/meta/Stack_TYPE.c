#include <core.h>

#include "Stack_TYPE.h"

Stack_TYPE *Stack_TYPE_create() {
    Stack_TYPE *stack = malloc(sizeof(Stack_TYPE));
    stack->head = NULL;
    stack->length = 0;
    return stack;
}

void Stack_TYPE_push(Stack_TYPE *stack, TYPE data) {
    stack->length += 1;
    if (!stack->head) {
        Node_TYPE *node = Node_TYPE_create(data);
        stack->head = node;
        return;
    }
    Node_TYPE *node = Node_TYPE_create(data);
    node->next = stack->head;
    stack->head = node;
}

Node_TYPE *Stack_TYPE_pop(Stack_TYPE *stack) {
    if (stack->length > 0) {
        stack->length -= 1;
    }
    if (stack->length == 0) {
        Node_TYPE *head_node = stack->head;
        stack->head = NULL;
        return head_node;
    }
    Node_TYPE *head_node = stack->head;
    stack->head = head_node->next;
    head_node->next = NULL;
    return head_node;
}

Node_TYPE *Stack_TYPE_peek(Stack_TYPE *stack) { 
    return stack->head;
}

void Stack_TYPE_destroy(Stack_TYPE **stack) {
    free(*stack);
    *stack = NULL;
}

void Stack_TYPE_print(Stack_TYPE *stack) { 
    Node_TYPE *iter = stack->head; 
    printf("Stack length = %llu\n", stack->length);
    while (iter) { 
        printf("stack: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    // printf("\n");
}
