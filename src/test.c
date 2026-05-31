#define SAHA_IMPLEMENTATION
#include <saha.h>

#define CORE_IMPL
#include <core.h>
bool i32_eq(i32 a, i32 b) { return a == b; }

template(Vec(i8));
template(Vec(i32));
template(Vec(u8));
template(Map(i32));
template(Map(vec3));
template(Node(i32));
template(List(i32));
template(BiNode(i32));
template(DList(i32));
template(Queue(i32));
template(Stack(i32));
template(Vec(vec3));
template(Vec(List_i32));
template(Map(Vec_i32));

#include "tests.h"
#include "vec3.h"

// Generated public APIs. Value types above must be complete before this block.
// #include <Array.h>
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
