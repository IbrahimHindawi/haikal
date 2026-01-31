#pragma once
#include <stdio.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string.h>
#include <core.h>
#include <stdalign.h>
#define max_alloc_size 0x10000000000

typedef struct memops_arena memops_arena;
struct memops_arena {
    u8 *base;
    u8 *cursor;
    u8 *previous;
    u64 used;
    u64 pagesize;
    u64 npages;
};

typedef struct memops_arena_temp memops_arena_temp;
struct memops_arena_temp {
    memops_arena *arena;
    // void *pos;
    u64 used;
};

memops_arena_temp memops_arena_temp_begin(memops_arena *arena);
void memops_arena_temp_end(memops_arena_temp arena_temp);

bool memops_is_power_of_two(uintptr_t x);
uintptr_t memops_align_forward(uintptr_t ptr, size_t align);
void memops_arena_initialize(memops_arena *arena);
#define memops_arena_push_struct(arena, type) (type *)memops_arena_push(arena, sizeof(type), haikal_alignof(type))
#define memops_arena_push_array(arena, type, count) (type *)memops_arena_push(arena, sizeof(type) * count, haikal_alignof(type))
void *memops_arena_push(memops_arena *arena, u64 alloc_size, u64 align);
#define memops_arena_push_array_zero(arena, type, count) (type *)memops_arena_push_zero(arena, sizeof(type) * count, haikal_alignof(type))
void *memops_arena_push_zero(memops_arena *arena, u64 alloc_size, u64 align);
void *memops_arena_set_pos(memops_arena *arena, void *pos);
#define memops_arena_pop_array(arena, type, count) (type *)memops_arena_pop(arena, sizeof(type) * count)
void *memops_arena_pop(memops_arena *arena, u64 alloc_size);
void *memops_arena_get_pos(memops_arena *arena);
void memops_arena_clear(memops_arena *arena);
#define memops_arena_realloc(arena, type, new_count, old_ptr, old_count) \
    (type *)memops_arena_realloc_(arena, sizeof(type) * new_count, old_ptr, sizeof(type) * old_count, haikal_alignof(type))
void *memops_arena_realloc_(memops_arena *arena, u64 new_alloc_size, void *old_ptr, u64 old_alloc_size, u64 align);
void memops_arena_destroy(memops_arena *arena);
void memops_arena_print(memops_arena *arena);

// #define SAHA_IMPLEMENTATION
#ifdef SAHA_IMPLEMENTATION

bool memops_is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

uintptr_t memops_align_forward(uintptr_t ptr, size_t align) {
	uintptr_t p;
    uintptr_t a;
    uintptr_t modulo;

	assert(memops_is_power_of_two(align));

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

void memops_arena_initialize(memops_arena *arena) {
    SYSTEM_INFO systeminfo = {0};
    GetSystemInfo(&systeminfo);
    // printf("allocationgranularity = %lu\n", systeminfo.dwAllocationGranularity);
    // printf("page size = %lu\n", systeminfo.dwPageSize);
    arena->pagesize = systeminfo.dwPageSize; // 4096 or 0x1000
    arena->base = (u8 *)VirtualAlloc(NULL, max_alloc_size, MEM_RESERVE, PAGE_NOACCESS); // reserve 1,099,511,627,776 bytes
    if (!arena->base) { exit(EXIT_FAILURE); }
    arena->cursor = arena->base;
    arena->previous = arena->base;
    arena->used = 0;
    arena->npages = 0;
}

void *memops_arena_push(memops_arena *arena, u64 alloc_size, u64 align) {
    //
    // Current aligned cursor based on: base + used
    //
    // printf("arena: alloc: %zu\n", alloc_size);
    uintptr_t base_addr = (uintptr_t)arena->base;
    uintptr_t curr_addr = base_addr + (uintptr_t)arena->used;

    uintptr_t aligned_addr = memops_align_forward(curr_addr, align);
    uintptr_t diff         = aligned_addr - curr_addr;

    u64 needed     = arena->used + alloc_size + diff;
    u64 committed  = arena->pagesize * arena->npages;

    //
    // COMMIT MISSING MEMORY
    //
    if (needed > committed) {
        u64 missing_bytes = needed - committed;
        u64 pages_to_commit =
            (missing_bytes + arena->pagesize - 1) / arena->pagesize;

        //
        // 🔥 CRITICAL FIX:
        // commit starting at the address inside reserved region:
        //
        uintptr_t commit_addr = base_addr + committed;

        void *r = VirtualAlloc(
            (void *)commit_addr,
            pages_to_commit * arena->pagesize,
            MEM_COMMIT,
            PAGE_READWRITE
        );

        if (!r) {
            printf("memops_arena commit failed at %p!\n", (void*)commit_addr);
            DebugBreak();
            exit(EXIT_FAILURE);
        }

        arena->npages += (u32)pages_to_commit;
    }

    //
    // Overflow protection
    //
    if (needed > max_alloc_size) {
        printf("memops_arena maximum size exceeded!\n");
        exit(EXIT_FAILURE);
    }

    //
    // Update arena state
    //
    arena->used    += alloc_size + diff;

    // cursor ALWAYS stored as pointer, never recalc from 'used'
    arena->previous  = arena->cursor;
    arena->cursor    = (u8 *)aligned_addr;
    void *oldpos      = arena->cursor;

    arena->cursor    += alloc_size;

    return oldpos;
}

void *memops_arena_push_zero(memops_arena *arena, u64 alloc_size, u64 align) {
    void *p = memops_arena_push(arena, alloc_size, align);
    memset(p, 0, (size_t)alloc_size);
    return p;
}

void *memops_arena_set_pos(memops_arena *arena, void *pos) {
    // u64 diff = (u64)arena->cursor - (u64)pos;
    // arena->used -= diff;
    // arena->cursor = (u8 *)pos;
    // return arena->cursor;
    arena->cursor = (u8 *)pos;
    arena->used = (u64)((u8 *)pos - arena->base);
    return arena->cursor;
}

void *memops_arena_pop(memops_arena *arena, u64 alloc_size) {
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

void *memops_arena_get_pos(memops_arena *arena) {
    // return arena->cursor;
    return arena->base + arena->used;
}

void memops_arena_clear(memops_arena *arena) {
    arena->cursor = arena->base;
    arena->used = 0;
}

void *memops_arena_realloc_(memops_arena *arena, u64 new_alloc_size, void *old_ptr, u64 old_alloc_size, u64 align) {
    void *new_ptr = memops_arena_push(arena, new_alloc_size, align);
    memcpy(new_ptr, old_ptr, old_alloc_size);
    // memset(old_ptr, 0, old_alloc_size);
    return new_ptr;
}

void memops_arena_destroy(memops_arena *arena) { }

void memops_arena_print(memops_arena *arena) {
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

memops_arena_temp memops_arena_temp_begin(memops_arena *arena) {
    memops_arena_temp t;
    t.arena = arena;
    // t.pos = memops_arena_get_pos(arena);
    t.used = arena->used;
    return t;
}

void memops_arena_temp_end(memops_arena_temp t) {
    // memops_arena_set_pos(t.arena, t.pos);
    t.arena->used = t.used;
    t.arena->cursor = t.arena->base + t.used;
}

#endif
