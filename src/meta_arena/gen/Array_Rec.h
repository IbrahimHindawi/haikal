#pragma once
/* Array_Rec */
#include <core.h>
#include <saha.h>

structdecl(Rec);

structdef(Array_Rec) {
    Rec *data;
    u64 length;
    u64 border;
};

Array_Rec Array_Rec_reserve(Arena *arena, u64 length);
void Array_Rec_destroy(Arena *arena, Array_Rec *array);
Rec *Array_Rec_resize(Arena *arena, Array_Rec *array);
Rec *Array_Rec_append(Arena *arena, Array_Rec *array, Rec elem);
int Array_Rec_is_empty(Arena *arena, Array_Rec *array);

