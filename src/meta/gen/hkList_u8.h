#pragma once
/* hkList_u8 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_u8.h"

structdef(hkList_u8) {
    hkNode_u8 *head;
    hkNode_u8 *tail;
    usize length;
};

hkList_u8 *hklist_u8_create() {
    hkList_u8 *list = malloc(sizeof(hkList_u8));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_u8_append(hkList_u8 *list, u8 data) {
    if (!list->head) {
        list->head = hknode_u8_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_u8_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_u8 *temp = hknode_u8_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_u8_destroy(hkList_u8 **list) {
    free(*list);
    *list = null;
}

