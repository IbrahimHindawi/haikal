#include <stdio.h>
#include <stdlib.h>

// #include "node.h"
#define buffersize 8

// passing a pointer to a pointer lets us
// manipulate the address of the pointer itself!
void str_alloc(char **buff, size_t size) {
    *buff = malloc(sizeof(char) * size);
    if(buff == NULL) {
        printf("Failed to alloc!\n");
        exit(-1);
    }
    return;
}

int main() {
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
    return 0;
}
