//-----------------------------------------------
// primitives
//-----------------------------------------------
// haikal@Vec:voidptr:p
// haikal@Vec:i8:p
// haikal@Vec:i32:p
// haikal@Vec:f32:p
// haikal@Vec:char:p
// haikal@Vec:u8:p
// haikal@Map:i32:p
// haikal@Map:u64:p
// haikal@Node:i32:p
// haikal@List:i32:p
// haikal@BiNode:i32:p
// haikal@DList:i32:p
// haikal@Queue:i32:p
// haikal@Stack:i32:p
//-----------------------------------------------
// structs
//-----------------------------------------------
// haikal@Vec:vec3:s
// haikal@Map:vec3:s
// haikal@Vec:List_i32:s
// haikal@Map:Vec_i8:s
// haikal@Map:Vec_i32:s
// haikal@Vec:string8:s
// haikal@Vec:string8slice:s
//-----------------------------------------------
// unions
//-----------------------------------------------
#define SAHA_IMPLEMENTATION
#include <saha.h>

#define CORE_IMPL
#include <core.h>
bool i32_eq(i32 a, i32 b) { return a == b; }

#include "string8.h"
#include "test_types.h"
#include "tests.h"

// Generated public APIs. Value types above must be complete before this block.
#include <Array.h>
#include <Vec.h>
#include <Node.h>
#include <List.h>
#include <BiNode.h>
#include <DList.h>
#include <Stack.h>
#include <Queue.h>
#include <Map.h>

i32 main(i32 argc, char *argv[]) {
    printf("haikal test begin...\n");
    memops_arena arena = {};
    memops_arena_initialize(&arena);
    tests_run(&arena);
    printf("haikal test end...\n");
    return 0;
}

// Generated implementations. Keep these in one translation unit.
#include <Vec.c>
#include <BiNode.c>
#include <DList.c>
#include <Map.c>
#include <List.c>
#include <Node.c>
#include <Stack.c>
#include <Queue.c>
