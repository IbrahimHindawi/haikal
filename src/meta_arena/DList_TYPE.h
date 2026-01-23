#pragma once
/* DList_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"
#include "BiNode_TYPE.h"

structdef(DList_TYPE) {
    BiNode_TYPE *head;
    BiNode_TYPE *tail;
    usize length;
};

DList_TYPE *DList_TYPE_create(memops_arena *arena);
void DList_TYPE_insert_at(memops_arena *arena, DList_TYPE *list, TYPE item, i32 index);
void DList_TYPE_append(memops_arena *arena, DList_TYPE *list, TYPE item);
void DList_TYPE_prepend(memops_arena *arena, DList_TYPE *list, TYPE item);
usize DList_TYPE_get_length(memops_arena *arena, DList_TYPE *list);
BiNode_TYPE *DList_TYPE_remove_node(memops_arena *arena, DList_TYPE *list, BiNode_TYPE *node);
BiNode_TYPE *DList_TYPE_remove(memops_arena *arena, DList_TYPE *list, TYPE item);
BiNode_TYPE *DList_TYPE_remove_at(memops_arena *arena, DList_TYPE *list, i32 index);
BiNode_TYPE *DList_TYPE_get_at(memops_arena *arena, DList_TYPE *list, i32 index);
void DList_TYPE_destroy(memops_arena *arena, DList_TYPE **list);
void DList_TYPE_print(memops_arena *arena, DList_TYPE *list);
