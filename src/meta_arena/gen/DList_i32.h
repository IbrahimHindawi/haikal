#pragma once
/* DList_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);
#include "BiNode_i32.h"

structdef(DList_i32) {
    BiNode_i32 *head;
    BiNode_i32 *tail;
    usize length;
};

DList_i32 *DList_i32_create(Arena *arena);
void DList_i32_insert_at(Arena *arena, DList_i32 *list, i32 item, i32 index);
void DList_i32_append(Arena *arena, DList_i32 *list, i32 item);
void DList_i32_prepend(Arena *arena, DList_i32 *list, i32 item);
usize DList_i32_get_length(Arena *arena, DList_i32 *list);
BiNode_i32 *DList_i32_remove_node(Arena *arena, DList_i32 *list, BiNode_i32 *node);
BiNode_i32 *DList_i32_remove(Arena *arena, DList_i32 *list, i32 item);
BiNode_i32 *DList_i32_remove_at(Arena *arena, DList_i32 *list, i32 index);
BiNode_i32 *DList_i32_get_at(Arena *arena, DList_i32 *list, i32 index);
void DList_i32_destroy(Arena *arena, DList_i32 **list);
void DList_i32_print(Arena *arena, DList_i32 *list);

