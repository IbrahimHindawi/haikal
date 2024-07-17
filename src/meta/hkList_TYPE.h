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
void hkList_TYPE_insert_at(hkList_TYPE *list, i32 index);
void hkList_TYPE_append(hkList_TYPE *list, TYPE item);
void hkList_TYPE_prepend(hkList_TYPE *list, TYPE item);
i32 hkList_TYPE_get_length(hkList_TYPE *list);
hkNode_TYPE *hkList_TYPE_remove(hkList_TYPE *list, TYPE item);
hkNode_TYPE *hkList_TYPE_remove_at(hkList_TYPE *list, i32 index);
hkNode_TYPE *hkList_TYPE_get_at(hkList_TYPE *list, i32 index);
void hkList_TYPE_destroy(hkList_TYPE **list);

#define HKLIST_IMPL 1
#ifdef HKLIST_IMPL
hkList_TYPE *hkList_TYPE_create() {
    hkList_TYPE *list = malloc(sizeof(hkList_TYPE));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void hkList_TYPE_insert_at(hkList_TYPE *list, i32 index) {
}

void hkList_TYPE_append(hkList_TYPE *list, TYPE item) {
    if (!list->head) {
        list->head = hkNode_TYPE_create(item);
        list->length += 1;
        return;
    }
    if (!list->tail) {
        list->tail = hkNode_TYPE_create(item);
        list->head->next = list->tail;
        list->length += 1;
        return;
    }
    hkNode_TYPE *temp = hkNode_TYPE_create(item);
    list->tail->next = temp; 
    list->tail = temp;
    list->length += 1;
}

void hkList_TYPE_prepend(hkList_TYPE *list, TYPE item) {
    hkNode_TYPE *prependnode = hkNode_TYPE_create(item);
    list->length += 1;
    if (!list->head) {
        list->head = prependnode;
        return;
    }
}

i32 hkList_TYPE_get_length(hkList_TYPE *list) {
    return list->length;
}

hkNode_TYPE *hkList_TYPE_remove(hkList_TYPE *list, TYPE item) {
    hkNode_TYPE *result = NULL;
    return result;
}

hkNode_TYPE *hkList_TYPE_remove_at(hkList_TYPE *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    hkNode_TYPE *result = NULL;
    hkNode_TYPE *iter = list->head;
    hkNode_TYPE *prev = NULL;
    i32 count = 0;
    while (iter != NULL) {
        if (count == index) {
            if (iter == list->head && !list->head->next) {
                result = list->head;
                list->head = NULL;
                list->length -= 1;
                return result;
            } else if (iter == list->head && list->head->next) {
                result = list->head;
                list->head = list->head->next;
                list->length -= 1;
                return result;
            } else {
                result = iter;
                if (!iter->next) {
                    prev->next = NULL;
                    list->length -= 1;
                    return result;
                } else {
                    prev->next = iter->next;
                    list->length -= 1;
                    return result;
                }
            }
        }
        prev = iter;
        iter = iter->next;
        count += 1;
    }
    return result;
}

hkNode_TYPE *hkList_TYPE_get_at(hkList_TYPE *list, i32 index) {
    if (list->length < index) {
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    hkNode_TYPE *result = NULL;
    hkNode_TYPE *iter = list->head;
    i32 count = 0;
    while (iter != NULL) {
        if (count == index) {
            return iter;
        }
        iter = iter->next;
        count += 1;
    }
    return result;
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
#undef HKLIST_IMPL
