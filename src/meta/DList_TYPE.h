#pragma once
/* DList_TYPE */
#include <core.h>

#include "TYPE.h"
#include "BiNode_TYPE.h"

structdef(DList_TYPE) {
    BiNode_TYPE *head;
    BiNode_TYPE *tail;
    usize length;
};

DList_TYPE *DList_TYPE_create();
void DList_TYPE_insert_at(DList_TYPE *list, TYPE item, i32 index);
void DList_TYPE_append(DList_TYPE *list, TYPE item);
void DList_TYPE_prepend(DList_TYPE *list, TYPE item);
usize DList_TYPE_get_length(DList_TYPE *list);
BiNode_TYPE *DList_TYPE_remove_node(DList_TYPE *list, BiNode_TYPE *node);
BiNode_TYPE *DList_TYPE_remove(DList_TYPE *list, TYPE item);
BiNode_TYPE *DList_TYPE_remove_at(DList_TYPE *list, i32 index);
BiNode_TYPE *DList_TYPE_get_at(DList_TYPE *list, i32 index);
void DList_TYPE_destroy(DList_TYPE **list);
void DList_TYPE_print(DList_TYPE *list);
