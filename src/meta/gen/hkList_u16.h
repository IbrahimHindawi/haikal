#pragma once
/* hkList_u16 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u16.h"

structdef(hkList_u16) {
    hkNode_u16 *head;
    hkNode_u16 *tail;
    usize length;
};

hkList_u16 *hklist_u16_create() {
    hkList_u16 *list = malloc(sizeof(hkList_u16));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_u16_append(hkList_u16 *list, u16 data) {
    if (!list->head) {
        list->head = hknode_u16_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_u16_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_u16 *temp = hknode_u16_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_u16_destroy(hkList_u16 **list) {
    free(*list);
    *list = null;
}

