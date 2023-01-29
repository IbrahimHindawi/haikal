#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void ddestroy(void *data) {
    free(data);
}

int main() {
    List *linked_list = NULL;
    if ((linked_list = (List *)malloc(sizeof(List))) == NULL) {
        return -1;
    }
    list_init(linked_list, ddestroy);
    return 0;
}
