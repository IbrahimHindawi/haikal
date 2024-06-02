#pragma once
/* hkList_i16 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i16.h"

structdef(hkList_i16) {
    hkNode_i16 *head;
    hkNode_i16 *tail;
    usize length;
};

hkList_i16 *hklist_i16_create() {
    hkList_i16 *list = malloc(sizeof(hkList_i16));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_i16_append(hkList_i16 *list, i16 data) {
    if (!list->head) {
        list->head = hknode_i16_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_i16_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_i16 *temp = hknode_i16_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_i16_destroy(hkList_i16 **list) {
    free(*list);
    *list = null;
}

