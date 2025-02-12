#include <core.h>

#include "BiNode_i32.h"

BiNode_i32 *BiNode_i32_create(Arena *arena, i32 data) {
    // BiNode_i32 *node = malloc(sizeof(BiNode_i32));
    BiNode_i32 *node = arenaPushStruct(arena, BiNode_i32);
    if (node) {
        node->next = NULL;
        node->prev = NULL;
        node->data = data;
    }
    return node;
}

void BiNode_i32_destroy(Arena *arena, BiNode_i32 **node) {
    // free(*node);
    *node = NULL;
}

i32 BiNode_i32_value(Arena *arena, BiNode_i32 *node) {
    i32 result;
    if (node) { 
        result = node->data; 
    } else {  
        printf("BiNode_i32 is NULL.\n"); 
        exit(1);
    };
    return result;
}

