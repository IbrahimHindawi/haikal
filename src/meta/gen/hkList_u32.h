#pragma once
/* hkList_u32 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u32.h"

structdef(hkList_u32) {
    hkNode_u32 *head;
    hkNode_u32 *tail;
    usize length;
};

hkList_u32 *hklist_u32_create() {
    hkList_u32 *list = malloc(sizeof(hkList_u32));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_u32_append(hkList_u32 *list, u32 data) {
    if (!list->head) {
        list->head = hknode_u32_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_u32_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_u32 *temp = hknode_u32_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_u32_destroy(hkList_u32 **list) {
    free(*list);
    *list = null;
}

