#pragma once
/* hkList_f32 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_f32.h"

structdef(hkList_f32) {
    hkNode_f32 *head;
    hkNode_f32 *tail;
    usize length;
};

hkList_f32 *hklist_f32_create() {
    hkList_f32 *list = malloc(sizeof(hkList_f32));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_f32_append(hkList_f32 *list, f32 data) {
    if (!list->head) {
        list->head = hknode_f32_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_f32_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_f32 *temp = hknode_f32_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_f32_destroy(hkList_f32 **list) {
    free(*list);
    *list = null;
}

