#include <stdio.h>
#include <stdlib.h>

#define buffersize 8

// pointers only allocate space for themselves when declared
// to allocate data for what they point to, we have 2 options
// by allocating storage by declaration (stack or static: char *bytes[1024])
// by allocating storage dynamically at runtime (ex: using malloc or realloc)

// automatic stack based variables have scope limited lifetimes
// which can result in dangling pointers on scope exit
void proc(int **iptr) {
    int a = 10;
    *iptr = &a;
    return;
}

void proc2(int *iptr) {
    int a = 10;
    iptr = &a;
    return;
}

// passing a pointer to a pointer lets us
// manipulate the address of the pointer itself!
// manual heap based variables longer lifetimes
// which can result in memory leaks they must be freed manually
void str_alloc(char **buff, size_t size) {
    *buff = malloc(sizeof(char) * size);
    if(buff == NULL) {
        printf("Failed to alloc!\n");
        exit(-1);
    }
    return;
}

int pointers_proc() {
    int *jptr = NULL;
    proc(&jptr);
    proc2(jptr);
    *jptr += 1;
    printf("%d\n", *jptr);

    char *strbuff = NULL;
    str_alloc(&strbuff, buffersize);

    // maybe use putchar and skip the \0 NUL char
    // since we already have the buffer size!
    strbuff[buffersize - 1] = '\0';
    char *p = strbuff;
    for (int i = 0; i < buffersize - 1; i += 1) {
        *p = 0x51;
        p += 1;
    }
    printf("strbuff = %s.\n", strbuff);
    free(strbuff);
    strbuff = NULL;

    int x = 8;
    int *q = &x;
    // q can be mutated:
    // q = (int *)3; q = &x;
    // q can be dereferenced: *q = ?
    // *q = 3;
    int **r = &q;
    // r can be mutated:
    // r = 0; r = &q;
    // r can be dereferenced: *r = ?
    // *r = (int *)3;
    // **r = 3;
    printf("address of x = %p, value of x = %d.\n", &x, x);
    printf("address of q = %p, value of q = %p, dereference *q = %d.\n", &q, q, *q);
    printf("address of r = %p, value of r = %p, dereference *r = %p.\n", &r, r, *r);

    *jptr += 1;
    printf("%d\n", *jptr);

    for (int x = 0; x < 10000000000; x += 1) {
        *jptr = 0;
        jptr += 1;
    }

    return 0;
}
