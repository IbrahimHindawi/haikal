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

#define HKSTACK_IMPL
#ifdef HKSTACK_IMPL

hkStack_TYPE *hkStack_TYPE_create() {
    hkStack_TYPE *stack = malloc(sizeof(hkStack_TYPE));
    stack->head = NULL;
    stack->length = 0;
    return stack;
}

void hkStack_TYPE_push(hkStack_TYPE *stack, TYPE data) {
    stack->length += 1;
    if (!stack->head) {
        hkNode_TYPE *node = hkNode_TYPE_create(data);
        stack->head = node;
        return;
    }
    hkNode_TYPE *node = hkNode_TYPE_create(data);
    node->next = stack->head;
    stack->head = node;
}

hkNode_TYPE *hkStack_TYPE_pop(hkStack_TYPE *stack) {
    if (stack->length > 0) {
        stack->length -= 1;
    }
    if (stack->length == 0) {
        hkNode_TYPE *head_node = stack->head;
        stack->head = NULL;
        return head_node;
    }
    hkNode_TYPE *head_node = stack->head;
    stack->head = head_node->next;
    head_node->next = NULL;
    return head_node;
}

hkNode_TYPE *hkStack_TYPE_peek(hkStack_TYPE *stack) { 
    return stack->head;
}

void hkStack_TYPE_destroy(hkStack_TYPE **stack) {
    free(*stack);
    *stack = NULL;
}

void hkStack_TYPE_print(hkStack_TYPE *stack) { 
    hkNode_TYPE *iter = stack->head; 
    printf("Queue length = %llu\n", stack->length);
    while (iter) { 
        printf("stack: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    // printf("\n");
}

#endif
