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

List_TYPE *List_TYPE_create(Arena *arena);
void List_TYPE_insert_at(Arena *arena, List_TYPE *list, TYPE item, i32 index);
void List_TYPE_append(Arena *arena, List_TYPE *list, TYPE item);
void List_TYPE_prepend(Arena *arena, List_TYPE *list, TYPE item);
usize List_TYPE_get_length(Arena *arena, List_TYPE *list);
Node_TYPE *List_TYPE_remove(Arena *arena, List_TYPE *list, TYPE item);
Node_TYPE *List_TYPE_remove_at(Arena *arena, List_TYPE *list, i32 index);
Node_TYPE *List_TYPE_get_at(Arena *arena, List_TYPE *list, i32 index);
void List_TYPE_destroy(Arena *arena, List_TYPE *list);
void List_TYPE_print(Arena *arena, List_TYPE *list);
