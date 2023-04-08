#include "stack.h"

#define T stack_t
void stack_init(T *stack, size_t stack_size, size_t element_size) {
    stack->stack_mem = (char *)malloc(stack_size * element_size);
    if (!stack->stack_mem) {
        printf("Memory Allocation Failed!\n");
        return;
    }
    stack->stack_size = stack_size;
    stack->element_size = element_size;
    stack->cursor = stack->stack_mem;
}

void stack_deinit(T *stack) {
    stack->element_size = 0;
    stack->stack_size = 0;
    free(stack->stack_mem);
    free(stack);
}

void stack_push(T *stack, void *item) {
    // printf("%p\n", (*stack)->cursor);
    // printf("%p\n", (*stack)->stack_mem);
    int pointer_difference = stack->cursor - stack->stack_mem;
    printf("ptr diff = %d, count = %d.\n", pointer_difference, count);
    if (pointer_difference >= stack->stack_size * stack->element_size) {
        printf("Stack Overflow!\n");
        exit(-1);
    }
    memcpy(stack->stack_mem, item, stack->element_size);
    stack->cursor += stack->element_size;
}

void stack_pop(T *stack) {
    stack->cursor -= stack->element_size;
}

void *stack_peek(T *stack) {
    return stack->cursor - stack->element_size;
}
#undef T
//-------------------------------------------------------------------------------
#define stack_size 256

int main()
{
    stack_t *s = NULL;
    s = (stack_t *)malloc(sizeof(stack_t));
    if (!s) {
        printf("Memory Allocation Failed!\n");
        return -1;
    }
    stack_init(s, 256, sizeof(int));
    if (!s) {
        printf("Memory Allocation Failed!\n");
        return -1;
    }
    
    /*
    {
        int x = 4;
        stack_push(&s, &x);
        int *y = (int *)stack_peek(&s);
        printf("%d", *y);
    }
    */
    
    for (int i = 0; i < stack_size + 1; i++) {
        int x = 4;
        stack_push(s, &x);
        ++count;
        // int *y = (int *)stack_peek(&s);
        // printf("%d", *y);
    }
    
    stack_deinit(s);
    free(s);
    s = NULL;
    return 0;
}

#undef stack_size
