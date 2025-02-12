#include <string.h>
#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <core.h>
#include <stdalign.h>
#define max_alloc_size 0x10000000000

bool isPowerOfTwo(uintptr_t x) {
	return (x & (x-1)) == 0;
}

uintptr_t memoryAlignForward(uintptr_t ptr, size_t align) {
	uintptr_t p;
    uintptr_t a;
    uintptr_t modulo;

	assert(isPowerOfTwo(align));

	p = ptr;
	a = (uintptr_t)align;
	// Same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);

	if (modulo != 0) {
		// If 'p' address is not aligned, push the address to the
		// next value which is aligned
		p += a - modulo;
	}
	return p;
}

typedef struct Arena Arena;
struct Arena {
    u8 *base;
    u8 *cursor;
    u8 *previous;
    u64 used;
    u64 pagesize;
    u64 npages;
};

void arenaInit(Arena *arena) {
    SYSTEM_INFO systeminfo = {0};
    GetSystemInfo(&systeminfo);
    // printf("allocationgranularity = %lu\n", systeminfo.dwAllocationGranularity);
    // printf("page size = %lu\n", systeminfo.dwPageSize);
    arena->pagesize = systeminfo.dwPageSize; // 4096 or 0x1000
    arena->base = VirtualAlloc(NULL, max_alloc_size, MEM_RESERVE, PAGE_NOACCESS); // reserve 1,099,511,627,776 bytes
    if (!arena->base) { exit(EXIT_FAILURE); }
    arena->cursor = arena->base;
    arena->previous = arena->base;
}

void *arenaPush(Arena *arena, u64 alloc_size, u64 align) {
	// Align 'curr_offset' forward to the specified alignment
	uintptr_t curr_ptr = (uintptr_t)arena->base + (uintptr_t)arena->used;
	uintptr_t offset = memoryAlignForward(curr_ptr, align);
    // Change to relative offset
	// offset -= (uintptr_t)arena->base;
    // uintptr_t diff = curr_ptr - offset;
    uintptr_t diff = offset - curr_ptr;
    // printf("align = %llu\n", align);
    // printf("offset = %llu\n", offset);
    if (arena->used + alloc_size + diff > arena->pagesize * arena->npages) {
        i32 npages = (i32)(ceil((f32)(arena->used + alloc_size + diff) / arena->pagesize));
        arena->npages = npages;
        arena->base = VirtualAlloc(arena->base, arena->pagesize * arena->npages, MEM_COMMIT, PAGE_READWRITE);
        if (!arena->base) { exit(EXIT_FAILURE); }
    }
    if (arena->used > max_alloc_size) {
        printf("Memory allocation failure! Maximum memory reached!\n");
        exit(EXIT_FAILURE);
    }
    arena->used += alloc_size + diff;
    // save cursor before push
    arena->previous = arena->cursor;
    // align cursor
    arena->cursor += diff;
    // save aligned cursor
    void *oldpos = arena->cursor;
    // allocate
    arena->cursor += alloc_size;
    // return aligned cursor
    return oldpos;
}

void *arenaPushZero(Arena *arena, u64 alloc_size, u64 align) {
    arenaPush(arena, alloc_size, align);
    memcpy(arena->cursor, 0, alloc_size);
    return arena->cursor;
}

void *arenaSetPos(Arena *arena, void *pos) {
    u64 diff = (u64)arena->cursor - (u64)pos;
    arena->used -= diff;
    arena->cursor = pos;
    return arena->cursor;
}

void *arenaPop(Arena *arena, u64 alloc_size) {
    // leads to fragmentation, needs solution
    // uintptr_t diff = arena->cursor - arena->previous;
    // uintptr_t offset = diff - alloc_size;
    // uintptr_t oldprevious = arena->previous;
    // arena->cursor = arena->previous;
    // arena->cursor -= alloc_size + offset;
    arena->cursor -= alloc_size;
    arena->used -= alloc_size;
    return arena->cursor;
}

void *arenaGetPos(Arena *arena) {
    return arena->cursor;
}

void arenaClear(Arena *arena) {
    arena->cursor = arena->base;
    arena->used = 0;
}

#define arenaRealloc(arena, type, new_count, old_ptr, old_count) \
    arenaRealloc_(arena, sizeof(type) * new_count, old_ptr, sizeof(type) * old_count, _Alignof(type))
void *arenaRealloc_(Arena *arena, u64 new_alloc_size, void *old_ptr, u64 old_alloc_size, u64 align) {
    void *new_ptr = arenaPush(arena, new_alloc_size, align);
    memcpy(new_ptr, old_ptr, old_alloc_size);
    memset(old_ptr, 0, old_alloc_size);
    return new_ptr;
}

void arenaDestroy(Arena *arena) { }

void arenaPrint(Arena *arena) {
    printf("Memory Dump: %llu bytes allocated.\n", arena->used);
    printf("%p: ", arena->base);
    for (i32 i = 0; i < arena->used; ++i) {
        if(i % 8 == 0 && i != 0) {
            printf("\n");
            printf("%p: ", &arena->base[i]);
        }
        printf("%02x ", arena->base[i]);
    }
    printf("\nMemory Dump: End.\n");
}

char *strAlloc(Arena *arena, char *input_str) {
    u64 input_str_len = strlen(input_str) + 1;
    char *output_str = arenaPush(arena, sizeof(char) * input_str_len, _Alignof(char));
    memcpy(output_str, input_str, input_str_len);
    // output_str[0] = 'a';
    // for (i32 i = 0; i < input_str_len; ++i) {
    //     output_str[i] = input_str[i];
    // }
    return output_str;
}

void *strDealloc(Arena *arena, const char *input_str) {
    u64 input_str_len = strlen(input_str) + 1;
    return arenaPop(arena, input_str_len);
}

#define arenaPushStruct(arena, type) arenaPush(arena, sizeof(type), _Alignof(type))
#define arenaPushArray(arena, type, count) arenaPush(arena, sizeof(type) * count, _Alignof(type))
#define arenaPopArray(arena, type, count) arenaPop(arena, sizeof(type) * count)
#define arenaPushArrayZero(arena, type, count) arenaPushZero(arena, sizeof(type) * count, _Alignof(type))
