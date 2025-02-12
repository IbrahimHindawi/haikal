#pragma once
/* Array_List_i32 */
#include <core.h>
#include <saha.h>

structdecl(List_i32);

structdef(Array_List_i32) {
    List_i32 *data;
    u64 length;
    u64 border;
};

Array_List_i32 Array_List_i32_reserve(Arena *arena, u64 length);
void Array_List_i32_destroy(Arena *arena, Array_List_i32 *array);
List_i32 *Array_List_i32_resize(Arena *arena, Array_List_i32 *array);
List_i32 *Array_List_i32_append(Arena *arena, Array_List_i32 *array, List_i32 elem);
int Array_List_i32_is_empty(Arena *arena, Array_List_i32 *array);

