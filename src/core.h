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
#define pi 3.141592f
#define sizeofarray(array) (sizeof(array) / sizeof(array[0]))
#define structdef(structname) typedef struct structname structname; struct structname
