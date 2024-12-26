#pragma once
/* List_TYPE */
#include <core.h>

#include "TYPE.h"
#include "Node_TYPE.h"

structdef(List_TYPE) {
    Node_TYPE *head;
    usize length;
};

List_TYPE *List_TYPE_create(void);
void List_TYPE_insert_at(List_TYPE *list, TYPE item, i32 index);
void List_TYPE_append(List_TYPE *list, TYPE item);
void List_TYPE_prepend(List_TYPE *list, TYPE item);
usize List_TYPE_get_length(List_TYPE *list);
Node_TYPE *List_TYPE_remove(List_TYPE *list, TYPE item);
Node_TYPE *List_TYPE_remove_at(List_TYPE *list, i32 index);
Node_TYPE *List_TYPE_get_at(List_TYPE *list, i32 index);
void List_TYPE_destroy(List_TYPE *list);
void List_TYPE_print(List_TYPE *list);
