#pragma once
/* hkList_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkNode_TYPE.h"

structdef(hkList_TYPE) {
    hkNode_TYPE *head;
    usize length;
};

hkList_TYPE *hkList_TYPE_create();
void hkList_TYPE_insert_at(hkList_TYPE *list, TYPE item, i32 index);
void hkList_TYPE_append(hkList_TYPE *list, TYPE item);
void hkList_TYPE_prepend(hkList_TYPE *list, TYPE item);
usize hkList_TYPE_get_length(hkList_TYPE *list);
hkNode_TYPE *hkList_TYPE_remove(hkList_TYPE *list, TYPE item);
hkNode_TYPE *hkList_TYPE_remove_at(hkList_TYPE *list, i32 index);
hkNode_TYPE *hkList_TYPE_get_at(hkList_TYPE *list, i32 index);
void hkList_TYPE_destroy(hkList_TYPE **list);
void hkList_TYPE_print(hkList_TYPE *list);

// #define HKLIST_IMPL
#ifdef HKLIST_IMPL
hkList_TYPE *hkList_TYPE_create() {
    hkList_TYPE *list = malloc(sizeof(hkList_TYPE));
    list->head = NULL;
    list->length = 0;
    return list;
}

void hkList_TYPE_insert_at(hkList_TYPE *list, TYPE item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    } else if (list->length == index) {
        hkList_TYPE_append(list, item);
    } else if (index == 0) {
        hkList_TYPE_prepend(list, item);
    }
    list->length += 1;
    hkNode_TYPE *iter = list->head;
    i32 count = 0;
    while (iter) {
        if (count == index) {
            hkNode_TYPE *next_node = iter->next;
            hkNode_TYPE *new_node = hkNode_TYPE_create(item);
            if (next_node) {
                iter->next = new_node;
                new_node->next = next_node;
                return;
            } else {
                iter->next = new_node;
                return;
            }
        }
        iter = iter->next;
        count += 1;
    }
}

void hkList_TYPE_append(hkList_TYPE *list, TYPE item) {
    if (!list->head) {
        list->head = hkNode_TYPE_create(item);
        list->length += 1;
        return;
    }
    hkNode_TYPE *temp = hkNode_TYPE_create(item);
    list->length += 1;
}

void hkList_TYPE_prepend(hkList_TYPE *list, TYPE item) {
    hkNode_TYPE *prepend_node = hkNode_TYPE_create(item);
    list->length += 1;
    if (!list->head) {
        list->head = prepend_node;
        return;
    }
    prepend_node->next = list->head;
    list->head = prepend_node;
}

usize hkList_TYPE_get_length(hkList_TYPE *list) {
    return list->length;
}

hkNode_TYPE *hkList_TYPE_remove(hkList_TYPE *list, TYPE item) {
    if (list->length == 0) {
        return NULL;
    }
    hkNode_TYPE *result = NULL;
    hkNode_TYPE *iter = list->head;
    hkNode_TYPE *prev = NULL;
    while (iter) {
        if (TYPE_eq(iter->data, item)) {
            if (iter == list->head) {
                result = list->head;
                if (list->head->next) {
                    list->head = list->head->next;
                } else {
                    list->head = NULL;
                }
                list->length -= 1;
                return result;
            }
            result = iter;
            if(iter->next) {
                prev->next = iter->next;
            } else {
                prev->next = NULL;
            }
            list->length -= 1;
            return result;
        }
        prev = iter;
        iter = iter->next;
    }
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
    while (iter) {
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
    while (iter) {
        hkNode_TYPE *destroyer = iter;
        iter = iter->next;
        hkNode_TYPE_destroy(&destroyer);
    }
    free(*list);
    *list = NULL;
}

void hkList_TYPE_print(hkList_TYPE *list) { 
    hkNode_TYPE *iter = list->head; 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    printf("\n");
}

#endif
#undef HKLIST_IMPL
