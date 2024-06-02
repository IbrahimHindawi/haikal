#pragma once
/* hkList_i32 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i32.h"

structdef(hkList_i32) {
    hkNode_i32 *head;
    hkNode_i32 *tail;
    usize length;
};

hkList_i32 *hklist_i32_create() {
    hkList_i32 *list = malloc(sizeof(hkList_i32));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_i32_append(hkList_i32 *list, i32 data) {
    if (!list->head) {
        list->head = hknode_i32_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_i32_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_i32 *temp = hknode_i32_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_i32_destroy(hkList_i32 **list) {
    free(*list);
    *list = null;
}

