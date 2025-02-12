#pragma once
/* BiNode_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);

structdef(BiNode_i32) {
    BiNode_i32 *next;
    BiNode_i32 *prev;
    i32 data;
};

BiNode_i32 *BiNode_i32_create(Arena *arena, i32 data);
void BiNode_i32_destroy(Arena *arena, BiNode_i32 **node);
i32 BiNode_i32_value(Arena *arena, BiNode_i32 *node);

#define BiNode_i32_get(node, value) \
    if (!(node)) { \
        printf(#node \
                " was null!\n"); \
        /* debugbreak? */ \
    } else { \
        (value) = (node)->data; \
    }

