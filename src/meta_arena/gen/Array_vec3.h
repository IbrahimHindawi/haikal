#pragma once
/* Array_vec3 */
#include <core.h>
#include <saha.h>

structdecl(vec3);

structdef(Array_vec3) {
    vec3 *data;
    u64 length;
    u64 border;
};

Array_vec3 Array_vec3_reserve(Arena *arena, u64 length);
void Array_vec3_destroy(Arena *arena, Array_vec3 *array);
vec3 *Array_vec3_resize(Arena *arena, Array_vec3 *array);
vec3 *Array_vec3_append(Arena *arena, Array_vec3 *array, vec3 elem);
int Array_vec3_is_empty(Arena *arena, Array_vec3 *array);

