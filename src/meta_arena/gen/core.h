#pragma once

#ifdef _MSC_VER
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define null NULL

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;

typedef void *voidptr;
// typedef i8 *str;
// typedef const str *cstr;
typedef const char *str;
typedef str *strptr;

#define internal static
#define global static
#define localpersist static

#define Kilobytes(N) ((u64)(N) * 1024)
#define Megabytes(N) ((u64)Kilobytes(N) * 1024)
#define Gigabytes(N) ((u64)Megabytes(N) * 1024)

#define stringify(s) #s
#define concat(a, b) a##b
#define Pi 3.141592f
#define sizeofarray(array) (sizeof(array) / sizeof(array[0]))
#define cast(T, V) (T)(V)

#define primdecl(primname)
#define enumdecl(enumname) typedef enum enumname enumname
#define enumdef(enumname) typedef enum enumname enumname; enum enumname
#define uniondecl(unionname) typedef union unionname unionname
#define uniondef(unionname) typedef union unionname unionname; union unionname
#define structdecl(structname) typedef struct structname structname
#define structdef(structname) typedef struct structname structname; struct structname

#define def_eq_prot(T) bool T##_eq(T a, T b);
#define def_eq_body(T) bool T##_eq(T a, T b) { return a == b; }

def_eq_prot(f32);
def_eq_prot(f64);
def_eq_prot(u8);
def_eq_prot(u16);
def_eq_prot(u32);
def_eq_prot(u64);
def_eq_prot(i8);
def_eq_prot(i16);
def_eq_prot(i32);
def_eq_prot(i64);
def_eq_prot(usize);

#ifdef CORE_IMPL
def_eq_body(f32);
def_eq_body(f64);
def_eq_body(u8);
def_eq_body(u16);
def_eq_body(u32);
def_eq_body(u64);
def_eq_body(i8);
def_eq_body(i16);
def_eq_body(i32);
def_eq_body(i64);
def_eq_body(usize);
#endif
