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
typedef const char * str;
typedef str * strptr;
// typedef char * cstr;
// typedef str * cstrptr;

#define stringify(s) #s
#define concat(a, b) a##b
#define pi 3.141592f
#define sizeofarray(array) (sizeof(array) / sizeof(array[0]))
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
def_eq_prot(str);
def_eq_prot(strptr);

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
def_eq_body(str);
def_eq_body(strptr);
#endif
