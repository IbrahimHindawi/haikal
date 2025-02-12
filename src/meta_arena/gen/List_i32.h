#pragma once
/* List_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);
#include "Node_i32.h"

structdef(List_i32) {
    Node_i32 *head;
    usize length;
};

List_i32 *List_i32_create(Arena *arena);
void List_i32_insert_at(Arena *arena, List_i32 *list, i32 item, i32 index);
void List_i32_append(Arena *arena, List_i32 *list, i32 item);
void List_i32_prepend(Arena *arena, List_i32 *list, i32 item);
usize List_i32_get_length(Arena *arena, List_i32 *list);
Node_i32 *List_i32_remove(Arena *arena, List_i32 *list, i32 item);
Node_i32 *List_i32_remove_at(Arena *arena, List_i32 *list, i32 index);
Node_i32 *List_i32_get_at(Arena *arena, List_i32 *list, i32 index);
void List_i32_destroy(Arena *arena, List_i32 *list);
void List_i32_print(Arena *arena, List_i32 *list);

