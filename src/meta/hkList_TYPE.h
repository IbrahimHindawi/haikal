#pragma once
/* hkList_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkNode_TYPE.h"

structdef(hkList_TYPE) {
    hkNode_TYPE *head;
    hkNode_TYPE *tail;
    usize length;
};

hkList_TYPE *hkList_TYPE_create();
void hkList_TYPE_append(hkList_TYPE *list, TYPE data);
void hkList_TYPE_destroy(hkList_TYPE **list);

#ifdef HKLIST_IMPL

hkList_TYPE *hkList_TYPE_create() {
    hkList_TYPE *list = malloc(sizeof(hkList_TYPE));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void hkList_TYPE_append(hkList_TYPE *list, TYPE data) {
    if (!list->head) {
        list->head = hkNode_TYPE_create(data);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hkNode_TYPE_create(data);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_TYPE *temp = hkNode_TYPE_create(data);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hkList_TYPE_destroy(hkList_TYPE **list) {
    hkNode_TYPE *iter = (*list)->head;
    while (iter != NULL) {
        hkNode_TYPE *destroyer = iter;
        iter = iter->next;
        hkNode_TYPE_destroy(&destroyer);
    }
    free(*list);
    *list = NULL;
}

#endif
