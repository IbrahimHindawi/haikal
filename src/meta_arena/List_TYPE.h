#pragma once
/* List_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"
#include "Node_TYPE.h"

structdef(List_TYPE) {
    Node_TYPE *head;
    usize length;
};

List_TYPE *List_TYPE_create(memops_arena *arena);
void List_TYPE_insert_at(memops_arena *arena, List_TYPE *list, TYPE item, i32 index);
void List_TYPE_append(memops_arena *arena, List_TYPE *list, TYPE item);
void List_TYPE_prepend(memops_arena *arena, List_TYPE *list, TYPE item);
usize List_TYPE_get_length(memops_arena *arena, List_TYPE *list);
Node_TYPE *List_TYPE_remove(memops_arena *arena, List_TYPE *list, TYPE item);
Node_TYPE *List_TYPE_remove_at(memops_arena *arena, List_TYPE *list, i32 index);
Node_TYPE *List_TYPE_get_at(memops_arena *arena, List_TYPE *list, i32 index);
void List_TYPE_destroy(memops_arena *arena, List_TYPE *list);
void List_TYPE_print(memops_arena *arena, List_TYPE *list);
