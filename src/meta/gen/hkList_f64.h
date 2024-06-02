#pragma once
/* hkList_f64 */
#include "../core.h"
#include <stdlib.h>


#include "hkNode_f64.h"

structdef(hkList_f64) {
    hkNode_f64 *head;
    hkNode_f64 *tail;
    usize length;
};

hkList_f64 *hklist_f64_create() {
    hkList_f64 *list = malloc(sizeof(hkList_f64));
    list->head = null;
    list->tail = null;
    list->length = 0;
    return list;
}

void hklist_f64_append(hkList_f64 *list, f64 data) {
    if (!list->head) {
        list->head = hknode_f64_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hknode_f64_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_f64 *temp = hknode_f64_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hklist_f64_destroy(hkList_f64 **list) {
    free(*list);
    *list = null;
}

