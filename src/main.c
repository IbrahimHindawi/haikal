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
// for types that include a container of themselves eg `struct T { hkArray_T arr; };`
// the type must be included after the generated header.
// this is because the type needs to know the container definition.
// Warning: can be recursive type with `T *` but not `T`
//---------------------------------------------------------------------------------------------------
// primitives
//---------------------------------------------------------------------------------------------------
// haikal@hkArray:i8:p
// haikal@hkArray:i32:p
// haikal@hkHashMap:i32:p
// haikal@hkNode:i32:p
// haikal@hkList:i32:p
// haikal@hkBiNode:i32:p
// haikal@hkDList:i32:p
// haikal@hkQueue:i32:p
// haikal@hkStack:i32:p
//---------------------------------------------------------------------------------------------------
// structs
//---------------------------------------------------------------------------------------------------
// haikal@hkArray:hkList_i32:s
// haikal@hkArray:vec3:s
// haikal@hkArray:Rec:s
// haikal@hkHashMap:vec3:s
// haikal@hkHashMap:Rec:s
// haikal@hkHashMap:hkArray_i8:s
// haikal@hkHashMap:hkArray_i32:s

#define CORE_IMPL
#include <core.h>

#include "Arena.h"
#include "vec3.h"

#include <hkArray.h>
#include <hkNode.h>
#include <hkList.h>
#include <hkBiNode.h>
#include <hkDList.h>
#include <hkStack.h>
#include <hkQueue.h>

#include "Rec.h"
#include <hkHashMap.h>

#include "Component.h"

void hkArray_test() {
    printf("hkArray_test:\n");
    hkArray_i8 string = hkArray_i8_create(27);
    for (i32 i = 0; i < string.length; ++i) {
        string.data[i] = 0b01100000 | i + 1;
    }
    string.data[string.length - 1] = '\0';
    printf("string: %s\n", string.data);
    hkArray_i8_destroy(&string);

    hkArray_vec3 vectors = hkArray_vec3_create(10);
    for (i32 i = 0; i < vectors.length; ++i) {
        vectors.data[i].x = 1.0f;
        vectors.data[i].y = (f32)i;
        vectors.data[i].z = 3.141592f;
    }
    for (i32 i = 0; i < vectors.length; ++i) { 
        printf("vectors[%d] = {%f, %f, %f}\n", i, vectors.data[i].x, vectors.data[i].y, vectors.data[i].z); 
    }
    hkArray_vec3_destroy(&vectors);

    hkArray_i8 arr = {0};
    hkArray_i8_append(&arr, 127);
    hkArray_i8_append(&arr, 23);
    hkArray_i8_append(&arr, 11);
    hkArray_i8_append(&arr, 8);
    hkArray_i8_append(&arr, 127);
    hkArray_i8_append(&arr, 23);
    hkArray_i8_append(&arr, 11);
    hkArray_i8_append(&arr, 8);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    arr.length = 0;
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    hkArray_i8_append(&arr, 0xBA);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    hkArray_i8_destroy(&arr);
    printf("\n");
}

void hkList_test() {
    printf("hkList_test:\n");
    hkList_i32 loi = {0};
    hkNode_i32 *node = NULL;
    hkList_i32_append(&loi, 11);
    hkList_i32_append(&loi, 22);
    hkList_i32_append(&loi, 33);
    hkList_i32_append(&loi, 44);
    hkList_i32_print(&loi);

    node = hkList_i32_remove_at(&loi, 0);
    if (node) {
        hkNode_i32_destroy(&node);
    }
    hkList_i32_print(&loi);
    node = hkList_i32_remove_at(&loi, 1);
    if (node) {
        hkNode_i32_destroy(&node);
    }
    hkList_i32_print(&loi);
    node = hkList_i32_remove_at(&loi, 1);
    if (node) {
        hkNode_i32_destroy(&node);
    }
    hkList_i32_print(&loi);
    node = hkList_i32_remove_at(&loi, 0);
    if (node) {
        hkNode_i32_destroy(&node);
    }
    hkList_i32_print(&loi);
    if (loi.length == 0) {
        printf("list is empty\n");
    }
    hkList_i32_destroy(&loi);

    printf("hkArray_hkList_i32:\n");
    hkArray_hkList_i32 arrayoflists = {0};
    hkList_i32 *list = hkArray_hkList_i32_append(&arrayoflists, (hkList_i32) {0});
    if (!list) { printf("list invalid!\n"); }
    hkList_i32_append(list, 32);
    hkList_i32_append(list, 22);
    hkList_i32_append(list, 12);
    hkList_i32_print(list);
    list = hkArray_hkList_i32_append(&arrayoflists, (hkList_i32) {0});
    if (!list) { printf("list invalid!\n"); }
    hkList_i32_append(list, 16);
    hkList_i32_append(list, 26);
    hkList_i32_append(list, 36);
    hkList_i32_print(list);
    printf("array.length = %llu\n", arrayoflists.length);
    for (i32 i = 0; i < arrayoflists.length; ++i) {
        printf("list[%d] = \n", i);
        hkList_i32 list = arrayoflists.data[i];
        hkList_i32_print(&list);
    }
    printf("\n");
}

void hkDList_test() {
    printf("hkDList_test:\n");
    hkDList_i32 *loi = hkDList_i32_create();
    hkBiNode_i32 *node = NULL;
    hkDList_i32_append(loi, 11);
    hkDList_i32_append(loi, 22);
    hkDList_i32_append(loi, 33);
    hkDList_i32_append(loi, 44);
    hkDList_i32_print(loi);
    node = hkDList_i32_remove_at(loi, 0); if (node) { hkBiNode_i32_destroy(&node); }
    hkDList_i32_print(loi);
    node = hkDList_i32_remove_at(loi, 1); if (node) { hkBiNode_i32_destroy(&node); }
    hkDList_i32_print(loi);
    node = hkDList_i32_remove_at(loi, 1); if (node) { hkBiNode_i32_destroy(&node); }
    hkDList_i32_print(loi);
    hkDList_i32_destroy(&loi);
    printf("\n");
}

void hkQueue_test() {
    printf("hkQueue_test:\n");
    hkQueue_i32 *q = hkQueue_i32_create();
    hkQueue_i32_print(q);
    hkQueue_i32_enqueue(q, 0);
    hkQueue_i32_print(q);
    hkQueue_i32_enqueue(q, 1);
    hkQueue_i32_print(q);
    hkQueue_i32_enqueue(q, 2);
    hkQueue_i32_print(q);

    hkNode_i32 *node = NULL;
    i32 value = 0;

    node = hkQueue_i32_dequeue(q);
    hkNode_i32_get(node, value);
    printf("node value: %d\n", value);
    hkNode_i32_destroy(&node);
    hkQueue_i32_print(q);

    node = hkQueue_i32_dequeue(q);
    hkNode_i32_get(node, value);
    printf("node value: %d\n", value);
    hkNode_i32_destroy(&node);
    hkQueue_i32_print(q);

    node = hkQueue_i32_dequeue(q);
    hkNode_i32_get(node, value);
    printf("node value: %d\n", value);
    hkNode_i32_destroy(&node);
    hkQueue_i32_print(q);

    node = hkQueue_i32_dequeue(q);
    hkNode_i32_get(node, value);
    printf("node value: %d\n", value);
    hkNode_i32_destroy(&node);
    hkQueue_i32_print(q);

    hkQueue_i32_destroy(&q);
    printf("\n");
}

void hkStack_test() {
    printf("hkStack_test:\n");
    hkStack_i32 *stack = hkStack_i32_create();
    hkNode_i32 *node = NULL;
    hkStack_i32_push(stack, 32);
    hkStack_i32_push(stack, 12);
    hkStack_i32_push(stack, 22);
    hkStack_i32_push(stack, 42);
    hkStack_i32_print(stack);

    node = hkStack_i32_pop(stack);
    i32 value = 0;
    hkStack_i32_print(stack);

    node = hkStack_i32_pop(stack);
    hkNode_i32_get(node, value);
    hkStack_i32_print(stack);

    node = hkStack_i32_pop(stack);
    hkNode_i32_get(node, value);
    hkStack_i32_print(stack);

    node = hkStack_i32_pop(stack);
    hkNode_i32_get(node, value);
    hkStack_i32_print(stack);

    node = hkStack_i32_pop(stack);
    hkNode_i32_get(node, value);
    hkStack_i32_print(stack);

    hkStack_i32_destroy(&stack);
    printf("\n");
}

void hkHashMap_test() {
    printf("hkHashMap_test:\n");
    puts("");
    printf("hkHashMap_i32:\n");
    hkHashMap_i32 *hashmap = hkHashMap_i32_create();
    printf("hashmap length = %llu\n", hkHashMap_i32_length(hashmap));
    if (!hashmap) {
        printf("nomem\n");
        exit(-1);
    }
    if (!hkHashMap_i32_set(hashmap, "dog", 3)) {
        printf("nomem\n");
        exit(-1);
    }
    i32 *result = hkHashMap_i32_get(hashmap, "dog");
    if (result) {
        printf("key = %s, val = %d\n", "dog", *result);
    }
    printf("hashmap length = %llu\n", hkHashMap_i32_length(hashmap));
    hkHashMap_i32_destroy(hashmap);

    puts("");
    printf("hkHashMap_vec:\n");
    hkHashMap_vec3 *hashmapvec = hkHashMap_vec3_create();
    printf("hashmapvec length = %llu\n", hkHashMap_vec3_length(hashmapvec));
    if (!hashmapvec) {
        printf("nomem\n");
        exit(-1);
    }
    if (!hkHashMap_vec3_set(hashmapvec, "dog", (vec3){1.f, 0.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", hkHashMap_vec3_length(hashmapvec));
    if (!hkHashMap_vec3_set(hashmapvec, "frog", (vec3){0.f, 1.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", hkHashMap_vec3_length(hashmapvec));
    vec3 *resultvec = hkHashMap_vec3_get(hashmapvec, "dog");
    if (resultvec) {
        printf("key = %s, val = {%f, %f, %f}\n", "dog", resultvec->x, resultvec->y, resultvec->z);
    }
    printf("hashmapvec length = %llu\n", hkHashMap_vec3_length(hashmapvec));
    printf("hash iterator...\n");
    hkHashMapIterator_vec3 itvec = hkHashMapIterator_vec3_create(hashmapvec);
    while (hkHashMapIterator_vec3_next(&itvec)) {
        printf("key = %s, val = {%f, %f, %f}\n", itvec.key, itvec.val.x, itvec.val.y, itvec.val.z);
    }
    hkHashMap_vec3_destroy(hashmapvec);

    puts("");
    printf("hkHashMap_hkArray_i32:\n");
    hkHashMap_hkArray_i32 *hashmaparray = hkHashMap_hkArray_i32_create();
    hkArray_i32 *resultarray = hkHashMap_hkArray_i32_get(hashmaparray, "dog");
    if (!resultarray) {
        hkHashMap_hkArray_i32_set(hashmaparray, "dog", (hkArray_i32) {0});
        resultarray = hkHashMap_hkArray_i32_get(hashmaparray, "dog");
    }
    printf("key = %s, val = %p", "dog", resultarray);
    *resultarray = hkArray_i32_create(12);
    for (i32 i = 0; i < 12; i++) {
        resultarray->data[i] = i * i;
    }
    for (i32 i = 0; i < 12; i++) {
        printf("hkArray.data[%d] = %d\n", i, resultarray->data[i]);
    }
    printf("hashmapvec length = %llu\n", hkHashMap_hkArray_i32_length(hashmaparray));

    printf("hash iterator...\n");
    hkHashMapIterator_hkArray_i32 itarr = hkHashMapIterator_hkArray_i32_create(hashmaparray);
    while (hkHashMapIterator_hkArray_i32_next(&itarr)) {
        printf("key = %s, val = {%llu, %llu, %p}\n", itarr.key, itarr.val.length, itarr.val.border, itarr.val.data);
        hkArray_i32_destroy(&itarr.val);
    }
    hkHashMap_hkArray_i32_destroy(hashmaparray);
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
    hkArray_test();
    hkHashMap_test();
    hkList_test();
    hkDList_test();
    hkQueue_test();
    hkStack_test();
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

#include <hkArray.c>
#include <hkBiNode.c>
#include <hkDList.c>
#include <hkHashMap.c>
#include <hkList.c>
#include <hkNode.c>
#include <hkStack.c>
#include <hkQueue.c>
