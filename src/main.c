//---------------------------------------------------------------------------------------------------
// monomorphization codegen limitations:
//---------------------------------------------------------------------------------------------------
// for containers that have value types eg `T`,
// the type must be included before the generated header.
// this is because the container expects to know the type in it's struct.
// Warning: cannot be recursive type
//
// for containers that have pointer types eg `T *`,
// the type can be included before or after the generated header.
// this is because the container has `T` forward declared.
// Warning: can be recursive type
//
// for types that include a container of themselves eg `struct T { Array_T arr; };`
// the type must be included after the generated header.
// this is because the type needs to know the container definition.
// Warning: can be recursive type with `T *` but not `T`
//---------------------------------------------------------------------------------------------------
// primitives
//---------------------------------------------------------------------------------------------------
// haikal@Array:i8:p
// haikal@Array:i32:p
// haikal@HashMap:i32:p
// haikal@Node:i32:p
// haikal@List:i32:p
// haikal@BiNode:i32:p
// haikal@DList:i32:p
// haikal@Queue:i32:p
// haikal@Stack:i32:p
//---------------------------------------------------------------------------------------------------
// structs
//---------------------------------------------------------------------------------------------------
// haikal@Array:List_i32:s
// haikal@Array:vec3:s
// haikal@Array:Rec:s
// haikal@HashMap:vec3:s
// haikal@HashMap:Rec:s
// haikal@HashMap:Array_i8:s
// haikal@HashMap:Array_i32:s

#define CORE_IMPL
#include <core.h>

#include "Arena.h"
#include "vec3.h"

#include <Array.h>
#include <Node.h>
#include <List.h>
#include <BiNode.h>
#include <DList.h>
#include <Stack.h>
#include <Queue.h>

#include "Rec.h"
#include <HashMap.h>

#include "Component.h"

void Array_test() {
    printf("Array_test:\n");
    Array_i8 string = Array_i8_create(27);
    for (i32 i = 0; i < string.length; ++i) {
        string.data[i] = 0b01100000 | i + 1;
    }
    string.data[string.length - 1] = '\0';
    printf("string: %s\n", string.data);
    Array_i8_destroy(&string);

    Array_vec3 vectors = Array_vec3_create(10);
    for (i32 i = 0; i < vectors.length; ++i) {
        vectors.data[i].x = 1.0f;
        vectors.data[i].y = (f32)i;
        vectors.data[i].z = 3.141592f;
    }
    for (i32 i = 0; i < vectors.length; ++i) { 
        printf("vectors[%d] = {%f, %f, %f}\n", i, vectors.data[i].x, vectors.data[i].y, vectors.data[i].z); 
    }
    Array_vec3_destroy(&vectors);

    Array_i8 arr = {0};
    Array_i8_append(&arr, 127);
    Array_i8_append(&arr, 23);
    Array_i8_append(&arr, 11);
    Array_i8_append(&arr, 8);
    Array_i8_append(&arr, 127);
    Array_i8_append(&arr, 23);
    Array_i8_append(&arr, 11);
    Array_i8_append(&arr, 8);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    arr.length = 0;
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    Array_i8_append(&arr, 0xBA);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    Array_i8_destroy(&arr);
    printf("\n");
}

void List_test() {
    printf("List_test:\n");
    List_i32 loi = {0};
    Node_i32 *node = NULL;
    List_i32_append(&loi, 11);
    List_i32_append(&loi, 22);
    List_i32_append(&loi, 33);
    List_i32_append(&loi, 44);
    List_i32_print(&loi);

    node = List_i32_remove_at(&loi, 0);
    if (node) {
        Node_i32_destroy(&node);
    }
    List_i32_print(&loi);
    node = List_i32_remove_at(&loi, 1);
    if (node) {
        Node_i32_destroy(&node);
    }
    List_i32_print(&loi);
    node = List_i32_remove_at(&loi, 1);
    if (node) {
        Node_i32_destroy(&node);
    }
    List_i32_print(&loi);
    node = List_i32_remove_at(&loi, 0);
    if (node) {
        Node_i32_destroy(&node);
    }
    List_i32_print(&loi);
    if (loi.length == 0) {
        printf("list is empty\n");
    }
    List_i32_destroy(&loi);

    printf("Array_List_i32:\n");
    Array_List_i32 arrayoflists = {0};
    List_i32 *list = Array_List_i32_append(&arrayoflists, (List_i32) {0});
    if (!list) { printf("list invalid!\n"); }
    List_i32_append(list, 32);
    List_i32_append(list, 22);
    List_i32_append(list, 12);
    List_i32_print(list);
    list = Array_List_i32_append(&arrayoflists, (List_i32) {0});
    if (!list) { printf("list invalid!\n"); }
    List_i32_append(list, 16);
    List_i32_append(list, 26);
    List_i32_append(list, 36);
    List_i32_print(list);
    printf("array.length = %llu\n", arrayoflists.length);
    for (i32 i = 0; i < arrayoflists.length; ++i) {
        printf("list[%d] = \n", i);
        List_i32 list = arrayoflists.data[i];
        List_i32_print(&list);
    }
    printf("\n");
}

void DList_test() {
    printf("DList_test:\n");
    DList_i32 *loi = DList_i32_create();
    BiNode_i32 *node = NULL;
    DList_i32_append(loi, 11);
    DList_i32_append(loi, 22);
    DList_i32_append(loi, 33);
    DList_i32_append(loi, 44);
    DList_i32_print(loi);
    node = DList_i32_remove_at(loi, 0); if (node) { BiNode_i32_destroy(&node); }
    DList_i32_print(loi);
    node = DList_i32_remove_at(loi, 1); if (node) { BiNode_i32_destroy(&node); }
    DList_i32_print(loi);
    node = DList_i32_remove_at(loi, 1); if (node) { BiNode_i32_destroy(&node); }
    DList_i32_print(loi);
    DList_i32_destroy(&loi);
    printf("\n");
}

void Queue_test() {
    printf("Queue_test:\n");
    Queue_i32 *q = Queue_i32_create();
    Queue_i32_print(q);
    Queue_i32_enqueue(q, 0);
    Queue_i32_print(q);
    Queue_i32_enqueue(q, 1);
    Queue_i32_print(q);
    Queue_i32_enqueue(q, 2);
    Queue_i32_print(q);

    Node_i32 *node = NULL;
    i32 value = 0;

    node = Queue_i32_dequeue(q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(&node);
    Queue_i32_print(q);

    node = Queue_i32_dequeue(q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(&node);
    Queue_i32_print(q);

    node = Queue_i32_dequeue(q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(&node);
    Queue_i32_print(q);

    node = Queue_i32_dequeue(q);
    Node_i32_get(node, value);
    printf("node value: %d\n", value);
    Node_i32_destroy(&node);
    Queue_i32_print(q);

    Queue_i32_destroy(&q);
    printf("\n");
}

void Stack_test() {
    printf("Stack_test:\n");
    Stack_i32 *stack = Stack_i32_create();
    Node_i32 *node = NULL;
    Stack_i32_push(stack, 32);
    Stack_i32_push(stack, 12);
    Stack_i32_push(stack, 22);
    Stack_i32_push(stack, 42);
    Stack_i32_print(stack);

    node = Stack_i32_pop(stack);
    i32 value = 0;
    Stack_i32_print(stack);

    node = Stack_i32_pop(stack);
    Node_i32_get(node, value);
    Stack_i32_print(stack);

    node = Stack_i32_pop(stack);
    Node_i32_get(node, value);
    Stack_i32_print(stack);

    node = Stack_i32_pop(stack);
    Node_i32_get(node, value);
    Stack_i32_print(stack);

    node = Stack_i32_pop(stack);
    Node_i32_get(node, value);
    Stack_i32_print(stack);

    Stack_i32_destroy(&stack);
    printf("\n");
}

void HashMap_test() {
    printf("HashMap_test:\n");
    puts("");
    printf("HashMap_i32:\n");
    HashMap_i32 *hashmap = HashMap_i32_create();
    printf("hashmap length = %llu\n", HashMap_i32_length(hashmap));
    if (!hashmap) {
        printf("nomem\n");
        exit(-1);
    }
    if (!HashMap_i32_set(hashmap, "dog", 3)) {
        printf("nomem\n");
        exit(-1);
    }
    i32 *result = HashMap_i32_get(hashmap, "dog");
    if (result) {
        printf("key = %s, val = %d\n", "dog", *result);
    }
    printf("hashmap length = %llu\n", HashMap_i32_length(hashmap));
    HashMap_i32_destroy(hashmap);

    puts("");
    printf("HashMap_vec:\n");
    HashMap_vec3 *hashmapvec = HashMap_vec3_create();
    printf("hashmapvec length = %llu\n", HashMap_vec3_length(hashmapvec));
    if (!hashmapvec) {
        printf("nomem\n");
        exit(-1);
    }
    if (!HashMap_vec3_set(hashmapvec, "dog", (vec3){1.f, 0.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", HashMap_vec3_length(hashmapvec));
    if (!HashMap_vec3_set(hashmapvec, "frog", (vec3){0.f, 1.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", HashMap_vec3_length(hashmapvec));
    vec3 *resultvec = HashMap_vec3_get(hashmapvec, "dog");
    if (resultvec) {
        printf("key = %s, val = {%f, %f, %f}\n", "dog", resultvec->x, resultvec->y, resultvec->z);
    }
    printf("hashmapvec length = %llu\n", HashMap_vec3_length(hashmapvec));
    printf("hash iterator...\n");
    HashMapIterator_vec3 itvec = HashMapIterator_vec3_create(hashmapvec);
    while (HashMapIterator_vec3_next(&itvec)) {
        printf("key = %s, val = {%f, %f, %f}\n", itvec.key, itvec.val.x, itvec.val.y, itvec.val.z);
    }
    HashMap_vec3_destroy(hashmapvec);

    puts("");
    printf("HashMap_Array_i32:\n");
    HashMap_Array_i32 *hashmaparray = HashMap_Array_i32_create();
    Array_i32 *resultarray = HashMap_Array_i32_get(hashmaparray, "dog");
    if (!resultarray) {
        HashMap_Array_i32_set(hashmaparray, "dog", (Array_i32) {0});
        resultarray = HashMap_Array_i32_get(hashmaparray, "dog");
    }
    printf("key = %s, val = %p", "dog", resultarray);
    *resultarray = Array_i32_create(12);
    for (i32 i = 0; i < 12; i++) {
        resultarray->data[i] = i * i;
    }
    for (i32 i = 0; i < 12; i++) {
        printf("Array.data[%d] = %d\n", i, resultarray->data[i]);
    }
    printf("hashmapvec length = %llu\n", HashMap_Array_i32_length(hashmaparray));

    printf("hash iterator...\n");
    HashMapIterator_Array_i32 itarr = HashMapIterator_Array_i32_create(hashmaparray);
    while (HashMapIterator_Array_i32_next(&itarr)) {
        printf("key = %s, val = {%llu, %llu, %p}\n", itarr.key, itarr.val.length, itarr.val.border, itarr.val.data);
        Array_i32_destroy(&itarr.val);
    }
    HashMap_Array_i32_destroy(hashmaparray);
    printf("\n");
}

structdef(Payload) {
    i32 id;
    i32 mx;
    char *str;
};

structdef(vec4i8) { i8 x; i8 y; i8 z; i8 w; };

void Arena_test() {
    Arena arena = {0};
    arenaInit(&arena, store);

    const i32 len = 4;
    f32 *nums = arenaPushArray(&arena, i32, len);
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

    char *str0 = strAlloc(&arena, "this is a te");
    char *str1 = strAlloc(&arena, "st string to");
    char *str2 = strAlloc(&arena, "alloc bytes.");
    printf("%s\n", str0);
    printf("%s\n", str1);
    printf("%s\n", str2);

    strDealloc(&arena, str2);
    str2 = strAlloc(&arena, "fortitude");
    printf("%s\n", str0);
    printf("%s\n", str1);
    printf("%s\n", str2);

    Payload *pld = arenaPushStruct(&arena, Payload);
    pld->id = 0xDEADBEEF;
    pld->mx = 0xCAFEBABE;
    pld->str = "Name0";
    arenaPop(&arena, sizeof(Payload));
    pld = arenaPushStruct(&arena, Payload);
    pld->id = 0xFFFFFFFF;
    pld->mx = 0xFFFFFFFF;
    pld->str = "Name0";
    arenaPop(&arena, sizeof(Payload));

    arenaClear(&arena);

    // vec4i8 *vs = arenaPushArrayZero(&arena, vec4i8, 32);
    const i32 npts = 32;
    vec4i8 *vs = arenaPushArray(&arena, vec4i8, npts);
    for (i32 i = 0; i < npts; ++i) {
        vs[i].x = 0xAA;
        vs[i].y = 0xBB;
        vs[i].z = 0xCC;
        vs[i].w = 0xDD;
    }
    arenaPopArray(&arena, vec4i8, npts);

    printf("Memory Dump: %d bytes allocated.\n", N);
    printf("%p: ", store);
    for (i32 i = 0; i < N; ++i) {
        if(i % 8 == 0 && i != 0) {
            printf("\n");
            printf("%p: ", &store[i]);
        }
        printf("%02x ", store[i]);
    }
    arenaClear(&arena);
}

i32 main(i32 argc, char *argv[]) {
    printf("haikal test begin.\n");
    Array_test();
    HashMap_test();
    List_test();
    DList_test();
    Queue_test();
    Stack_test();
    // Arena_test();
    printf("haikal test end.\n");

    // TODO: fix code gen for external files
    // for this to work, we need to read all the included files
    // compile_commands.json should be enough...
    // or, use a unity build and just include everything in main.c 
    // but LSP will die in Component.h...
    //---------------------------------------------------------------------------------------------------
    // Component comp = Component_create(10);
    // printf("comp.id = %d\n", comp.id);
    // Component_destroy(&comp);

    return 0;
}

#include <Array.c>
#include <BiNode.c>
#include <DList.c>
#include <HashMap.c>
#include <List.c>
#include <Node.c>
#include <Stack.c>
#include <Queue.c>
