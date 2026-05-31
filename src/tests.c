#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

#include <core.h>
// #include "string8.h"
#include "vec3.h"

// #include <Array.h>
#include <Vec.h>
#include <Node.h>
#include <List.h>
#include <BiNode.h>
#include <DList.h>
#include <Stack.h>
#include <Queue.h>
#include <Map.h>

typedef struct Payload Payload;
struct Payload {
    i32 id;
    i32 mx;
    char *str;
};

typedef struct vec4i8 vec4i8;
struct vec4i8 {
    i8 x; i8 y; i8 z; i8 w;
};

static void arena_test(memops_arena *arena) {
    printf("arena_test:\n");
    printf("----------------------------\n");

    const i32 len = 4;
    f32 *nums = memops_arena_push_array(arena, f32, len);
    for (i32 i = 0; i < len; ++i) {
        nums[i] = (f32)(i + 1);
    }
    for (i32 i = 0; i < len; ++i) {
        printf("%f ", nums[i]);
    }
    printf("\n");

    u8 *ptr = (u8 *)nums;
    for (i32 i = 0; i < sizeof(f32) * len; ++i) {
        printf("%02x ", ptr[i]);
    }
    printf("\n");

    void *pos = arena->cursor;

    Payload *pld = memops_arena_push_struct(arena, Payload);
    pld->id = 0xDEADBEEF;
    pld->mx = 0xCAFEBABE;
    pld->str = "Name0";
    memops_arena_pop(arena, sizeof(Payload));
    pld = memops_arena_push_struct(arena, Payload);
    pld->id = 0xFFFFFFFF;
    pld->mx = 0xFFFFFFFF;
    pld->str = "Name0";
    memops_arena_pop(arena, sizeof(Payload));

    memops_arena_set_pos(arena, pos);
    i8 *x = memops_arena_push(arena, sizeof(i8), _Alignof(i8));
    *x = 0xDD;

    memops_arena_pop(arena, sizeof(Payload));

    memops_arena_set_pos(arena, pos);
    nums = memops_arena_push_array(arena, f32, len);
    for (i32 i = 0; i < len; ++i) {
        nums[i] = (f32)(i + 1);
    }

    memops_arena_clear(arena);

    const i32 npts = 32;
    vec4i8 *vs = memops_arena_push_array(arena, vec4i8, npts);
    for (i32 i = 0; i < npts; ++i) {
        vs[i].x = 0xAA;
        vs[i].y = 0xBB;
        vs[i].z = 0xCC;
        vs[i].w = 0xDD;
    }
    memops_arena_pop_array(arena, vec4i8, npts);

    memops_arena_clear(arena);
    printf("\n");
}

static void Vec_test(memops_arena *arena) {
    printf("Vec_test:\n");
    printf("----------------------------\n");
    void *pos = NULL;

    pos = memops_arena_get_pos(arena);
    i32 const vectors_alloc_capacity = 10;
    Vec_vec3 vectors = Vec_vec3_reserve(arena, vectors_alloc_capacity);
    for (i32 i = 0; i < vectors_alloc_capacity; ++i) {
        Vec_vec3_append(arena, &vectors, (vec3){1.f, (f32)i, 3.141592});
    }
    for (i32 i = 0; i < vectors.length; ++i) {
        printf("vectors[%d] = {%f, %f, %f}\n", i, vectors.data[i].x, vectors.data[i].y, vectors.data[i].z);
    }
    Vec_vec3_destroy(arena, &vectors);
    memops_arena_set_pos(arena, pos);

    Vec_i8 arr = {0};
    Vec_i8_reserve(arena, 32);
    Vec_i8_append(arena, &arr, 127);
    Vec_i8_append(arena, &arr, 23);
    Vec_i8_append(arena, &arr, 11);
    Vec_i8_append(arena, &arr, 8);
    Vec_i8_append(arena, &arr, 127);
    Vec_i8_append(arena, &arr, 23);
    Vec_i8_append(arena, &arr, 11);
    Vec_i8_append(arena, &arr, 8);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    arr.length = 0;
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    Vec_i8_append(arena, &arr, 0xBA);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    Vec_i8_destroy(arena, &arr);
    printf("\n");
}

// static void String_test(memops_arena *arena) {
//     void *pos = NULL;
//     pos = memops_arena_get_pos(arena);
//     i32 const string_alloc_capacity = 27;
//     string8 string = string8_reserve(arena, string_alloc_capacity);
//     for (i32 i = 0; i < string_alloc_capacity - 1; ++i) {
//         string8_append_byte(arena, &string, 0b01100000 | (i + 1));
//     }
//     printf("string = %s\n", string.data);
//     memops_arena_set_pos(arena, pos);
// 
//     pos = memops_arena_get_pos(arena);
//     string = string8_from_cstr(arena, "Hello, ");
//     string8_append_cstr(arena, &string, "World!");
//     string8_print(&string);
//     memops_arena_set_pos(arena, pos);
// 
//     pos = memops_arena_get_pos(arena);
//     string = string8_read_file(arena, "res/test.txt");
//     printf("file contents = %s\n", string.data);
//     Vec_string8slice lines = string8slice_split_from_string8(arena, string, '\n');
//     for (i32 i = 0; i < lines.length; i++) {
//         printf("[%d]:", i);
//         string8slice_print(lines.data[i]);
//     }
//     memops_arena_set_pos(arena, pos);
// 
//     pos = memops_arena_get_pos(arena);
//     string = string8_read_file(arena, "res/test.csv");
//     printf("file contents = \n%s\n", string.data);
//     lines = string8slice_split_from_string8(arena, string, '\n');
//     for (i32 i = 0; i < lines.length; i++) {
//         printf("[%d]:", i);
//         string8slice_print(lines.data[i]);
//         Vec_string8slice chunk = string8slice_split(arena, lines.data[i], ',');
//         void *innerpos = memops_arena_get_pos(arena);
//         for (i32 j = 0; j < chunk.length; j++) {
//             string8slice_print(chunk.data[j]);
//             f32 x = strtof(string8slice_to_cstr_temp(arena, chunk.data[j]), NULL);
//             memops_arena_set_pos(arena, innerpos);
//             printf("extracted float = %f\n", x);
//         }
//         printf("\n");
//     }
//     memops_arena_set_pos(arena, pos);
// }

static void List_test(memops_arena *arena) {
    printf("List_test:\n");
    List_i32 loi = {0};
    Node_i32 *node = NULL;
    List_i32_append(arena, &loi, 11);
    List_i32_append(arena, &loi, 22);
    List_i32_append(arena, &loi, 33);
    List_i32_append(arena, &loi, 44);
    List_i32_print(arena, &loi);

    node = List_i32_remove_at(arena, &loi, 0);
    if (node) {
        Node_i32_destroy(arena, &node);
    }
    List_i32_print(arena, &loi);
    node = List_i32_remove_at(arena, &loi, 1);
    if (node) {
        Node_i32_destroy(arena, &node);
    }
    List_i32_print(arena, &loi);
    node = List_i32_remove_at(arena, &loi, 1);
    if (node) {
        Node_i32_destroy(arena, &node);
    }
    List_i32_print(arena, &loi);
    node = List_i32_remove_at(arena, &loi, 0);
    if (node) {
        Node_i32_destroy(arena, &node);
    }
    List_i32_print(arena, &loi);
    if (loi.length == 0) {
        printf("list is empty\n");
    }
    List_i32_destroy(arena, &loi);

    printf("Vec_List_i32:\n");
    Vec_List_i32 arrayoflists = {0};
    List_i32 *list = Vec_List_i32_append(arena, &arrayoflists, (List_i32) {0});
    if (!list) { printf("list invalid!\n"); }
    List_i32_append(arena, list, 32);
    List_i32_append(arena, list, 22);
    List_i32_append(arena, list, 12);
    List_i32_print(arena, list);
    list = Vec_List_i32_append(arena, &arrayoflists, (List_i32) {0});
    if (!list) { printf("list invalid!\n"); }
    List_i32_append(arena, list, 16);
    List_i32_append(arena, list, 26);
    List_i32_append(arena, list, 36);
    List_i32_print(arena, list);
    printf("array.length = %llu\n", arrayoflists.length);
    for (i32 i = 0; i < arrayoflists.length; ++i) {
        printf("list[%d] = \n", i);
        List_i32 list = arrayoflists.data[i];
        List_i32_print(arena, &list);
    }
    printf("\n");
}

static void DList_test(memops_arena *arena) {
    printf("DList_test:\n");
    printf("----------------------------\n");
    DList_i32 *loi = DList_i32_create(arena);
    BiNode_i32 *node = NULL;
    DList_i32_append(arena, loi, 11);
    DList_i32_append(arena, loi, 22);
    DList_i32_append(arena, loi, 33);
    DList_i32_append(arena, loi, 44);
    DList_i32_print(arena, loi);
    node = DList_i32_remove_at(arena, loi, 0); if (node) { BiNode_i32_destroy(arena, &node); }
    DList_i32_print(arena, loi);
    node = DList_i32_remove_at(arena, loi, 1); if (node) { BiNode_i32_destroy(arena, &node); }
    DList_i32_print(arena, loi);
    node = DList_i32_remove_at(arena, loi, 1); if (node) { BiNode_i32_destroy(arena, &node); }
    DList_i32_print(arena, loi);
    DList_i32_destroy(arena, &loi);
    printf("\n");
}

static void Queue_test(memops_arena *arena) {
    printf("Queue_test:\n");
    printf("----------------------------\n");

    Queue_i32 *q = Queue_i32_create(arena);
    Queue_i32_print(arena, q);
    Queue_i32_enqueue(arena, q, 0);
    Queue_i32_print(arena, q);
    Queue_i32_enqueue(arena, q, 1);
    Queue_i32_print(arena, q);
    Queue_i32_enqueue(arena, q, 2);
    Queue_i32_print(arena, q);

    Node_i32 *node = NULL;
    i32 value = 0;

    node = Queue_i32_dequeue(arena, q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(arena, &node);
    Queue_i32_print(arena, q);

    node = Queue_i32_dequeue(arena, q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(arena, &node);
    Queue_i32_print(arena, q);

    node = Queue_i32_dequeue(arena, q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(arena, &node);
    Queue_i32_print(arena, q);

    node = Queue_i32_dequeue(arena, q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(arena, &node);
    Queue_i32_print(arena, q);

    Queue_i32_destroy(arena, &q);
    printf("\n");
}

static void Stack_test(memops_arena *arena) {
    printf("Stack_test:\n");
    printf("----------------------------\n");
    Stack_i32 *stack = Stack_i32_create(arena);
    Node_i32 *node = NULL;
    Stack_i32_push(arena, stack, 32);
    Stack_i32_push(arena, stack, 12);
    Stack_i32_push(arena, stack, 22);
    Stack_i32_push(arena, stack, 42);
    Stack_i32_print(arena, stack);

    node = Stack_i32_pop(arena, stack);
    i32 value = 0;
    Stack_i32_print(arena, stack);

    node = Stack_i32_pop(arena, stack);
    Node_i32_get(node, value);
    Stack_i32_print(arena, stack);

    node = Stack_i32_pop(arena, stack);
    Node_i32_get(node, value);
    Stack_i32_print(arena, stack);

    node = Stack_i32_pop(arena, stack);
    Node_i32_get(node, value);
    Stack_i32_print(arena, stack);

    node = Stack_i32_pop(arena, stack);
    Node_i32_get(node, value);
    Stack_i32_print(arena, stack);

    Stack_i32_destroy(arena, &stack);
    printf("\n");
}

static void Map_test(memops_arena *arena) {
    printf("Map_test:\n");
    puts("");
    printf("Map_i32:\n");
    Map_i32 *hashmap = Map_i32_create(arena);
    printf("hashmap length = %llu\n", Map_i32_length(arena, hashmap));
    if (!hashmap) {
        printf("nomem\n");
        exit(-1);
    }
    if (!Map_i32_set(arena, hashmap, "dog", 3)) {
        printf("nomem\n");
        exit(-1);
    }
    i32 *result = Map_i32_get(arena, hashmap, "dog");
    if (result) {
        printf("key = %s, val = %d\n", "dog", *result);
    }
    printf("hashmap length = %llu\n", Map_i32_length(arena, hashmap));
    Map_i32_destroy(arena, hashmap);

    puts("");
    printf("Map_vec:\n");
    Map_vec3 *hashmapvec = Map_vec3_create(arena);
    printf("hashmapvec length = %llu\n", Map_vec3_length(arena, hashmapvec));
    if (!hashmapvec) {
        printf("nomem\n");
        exit(-1);
    }
    if (!Map_vec3_set(arena, hashmapvec, "dog", (vec3){1.f, 0.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", Map_vec3_length(arena, hashmapvec));
    if (!Map_vec3_set(arena, hashmapvec, "frog", (vec3){0.f, 1.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", Map_vec3_length(arena, hashmapvec));
    vec3 *resultvec = Map_vec3_get(arena, hashmapvec, "dog");
    if (resultvec) {
        printf("key = %s, val = {%f, %f, %f}\n", "dog", resultvec->x, resultvec->y, resultvec->z);
    }
    printf("hashmapvec length = %llu\n", Map_vec3_length(arena, hashmapvec));
    printf("hash iterator...\n");
    MapIterator_vec3 itvec = MapIterator_vec3_create(arena, hashmapvec);
    while (MapIterator_vec3_next(arena, &itvec)) {
        printf("key = %s, val = {%f, %f, %f}\n", itvec.key, itvec.val.x, itvec.val.y, itvec.val.z);
    }
    Map_vec3_destroy(arena, hashmapvec);

    puts("");
    printf("Map_Vec_i32:\n");
    Map_Vec_i32 *hashmaparray = Map_Vec_i32_create(arena);
    Vec_i32 *resultarray = Map_Vec_i32_get(arena, hashmaparray, "dog");
    if (!resultarray) {
        Map_Vec_i32_set(arena, hashmaparray, "dog", (Vec_i32) {0});
        resultarray = Map_Vec_i32_get(arena, hashmaparray, "dog");
    }
    printf("key = %s, val = %p", "dog", resultarray);
    *resultarray = Vec_i32_reserve(arena, 12);
    for (i32 i = 0; i < 12; i++) {
        resultarray->data[i] = i * i;
    }
    for (i32 i = 0; i < 12; i++) {
        printf("Vec.data[%d] = %d\n", i, resultarray->data[i]);
    }
    printf("hashmapvec length = %llu\n", Map_Vec_i32_length(arena, hashmaparray));

    printf("hash iterator...\n");
    MapIterator_Vec_i32 itarr = MapIterator_Vec_i32_create(arena, hashmaparray);
    while (MapIterator_Vec_i32_next(arena, &itarr)) {
        printf("key = %s, val = {%llu, %llu, %p}\n", itarr.key, itarr.val.length, itarr.val.border, itarr.val.data);
        Vec_i32_destroy(arena, &itarr.val);
    }
    Map_Vec_i32_destroy(arena, hashmaparray);
    printf("\n");
}

void tests_run(memops_arena *arena) {
    // arena_test(arena);
    Vec_test(arena);
    // String_test(arena);
    Map_test(arena);
    List_test(arena);
    DList_test(arena);
    Queue_test(arena);
    Stack_test(arena);
}
