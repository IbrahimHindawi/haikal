#pragma once
/* hkList_i64 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i64.h"

structdef(hkList_i64) {
    hkNode_i64 *head;
    hkNode_i64 *tail;
    usize length;
};

hkList_i64 *hklist_i64_create() {
    hkList_i64 *list = malloc(sizeof(hkList_i64));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_i64_append(hkList_i64 *list, i64 data) {
    if (!list->head) {
        list->head = hknode_i64_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_i64_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_i64 *temp = hknode_i64_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_i64_destroy(hkList_i64 **list) {
    free(*list);
    *list = null;
}

