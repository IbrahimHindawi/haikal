#pragma once
/* hkList_i8 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_i8.h"

structdef(hkList_i8) {
    hkNode_i8 *head;
    hkNode_i8 *tail;
    usize length;
};

hkList_i8 *hklist_i8_create() {
    hkList_i8 *list = malloc(sizeof(hkList_i8));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_i8_append(hkList_i8 *list, i8 data) {
    if (!list->head) {
        list->head = hknode_i8_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_i8_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_i8 *temp = hknode_i8_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_i8_destroy(hkList_i8 **list) {
    free(*list);
    *list = null;
}

