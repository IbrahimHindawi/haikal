#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

#define sizeofarray(array, type) (sizeof(array) / sizeof(array[0]))

#define structdef(structname) typedef struct structname structname; struct structname
