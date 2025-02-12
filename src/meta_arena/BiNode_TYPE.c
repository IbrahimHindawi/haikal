#include <core.h>

#include "BiNode_TYPE.h"

BiNode_TYPE *BiNode_TYPE_create(Arena *arena, TYPE data) {
    // BiNode_TYPE *node = malloc(sizeof(BiNode_TYPE));
    BiNode_TYPE *node = arenaPushStruct(arena, BiNode_TYPE);
    if (node) {
        node->next = NULL;
        node->prev = NULL;
        node->data = data;
    }
    return node;
}

void BiNode_TYPE_destroy(Arena *arena, BiNode_TYPE **node) {
    // free(*node);
    *node = NULL;
}

TYPE BiNode_TYPE_value(Arena *arena, BiNode_TYPE *node) {
    TYPE result;
    if (node) { 
        result = node->data; 
    } else {  
        printf("BiNode_TYPE is NULL.\n"); 
        exit(1);
    };
    return result;
}
