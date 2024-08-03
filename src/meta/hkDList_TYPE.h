#pragma once
/* hkDList_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkBiNode_TYPE.h"

structdef(hkDList_TYPE) {
    hkBiNode_TYPE *head;
    hkBiNode_TYPE *tail;
    usize length;
};

hkDList_TYPE *hkDList_TYPE_create();
void hkDList_TYPE_insert_at(hkDList_TYPE *list, TYPE item, i32 index);
void hkDList_TYPE_append(hkDList_TYPE *list, TYPE item);
void hkDList_TYPE_prepend(hkDList_TYPE *list, TYPE item);
usize hkDList_TYPE_get_length(hkDList_TYPE *list);
hkBiNode_TYPE *hkDList_TYPE_remove_node(hkDList_TYPE *list, hkBiNode_TYPE *node);
hkBiNode_TYPE *hkDList_TYPE_remove(hkDList_TYPE *list, TYPE item);
hkBiNode_TYPE *hkDList_TYPE_remove_at(hkDList_TYPE *list, i32 index);
hkBiNode_TYPE *hkDList_TYPE_get_at(hkDList_TYPE *list, i32 index);
void hkDList_TYPE_destroy(hkDList_TYPE **list);
void hkDList_TYPE_print(hkDList_TYPE *list);

// #define HKDLIST_IMPL
#ifdef HKDLIST_IMPL
hkDList_TYPE *hkDList_TYPE_create() {
    hkDList_TYPE *list = malloc(sizeof(hkDList_TYPE));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void hkDList_TYPE_insert_at(hkDList_TYPE *list, TYPE item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    }
    if (list->length == index) {
        hkDList_TYPE_append(list, item);
    } else if (index == 0) {
        hkDList_TYPE_prepend(list, item);
    }
    list->length += 1;
    hkBiNode_TYPE *iter = hkDList_TYPE_get_at(list, index);
    hkBiNode_TYPE *node = hkBiNode_TYPE_create(item);
    node->next = iter;
    node->prev = iter->prev;
    iter->prev = node;
    if (iter->prev) {
        iter->prev->next = iter;
    }
}

void hkDList_TYPE_append(hkDList_TYPE *list, TYPE item) {
    if (!list->head) {
        list->length += 1;
        list->head = hkBiNode_TYPE_create(item);
        list->tail = list->head;
        return;
    }
    if (!list->tail) {
        list->length += 1;
        list->tail = hkBiNode_TYPE_create(item);
        list->head->next = list->tail;
        return;
    }
    list->length += 1;
    hkBiNode_TYPE *node = hkBiNode_TYPE_create(item);
    node->prev = list->tail;
    list->tail->next = node; 
    list->tail = node;
}

void hkDList_TYPE_prepend(hkDList_TYPE *list, TYPE item) {
    hkBiNode_TYPE *node = hkBiNode_TYPE_create(item);
    list->length += 1;
    if (!list->head) {
        list->head = list->tail = node;
        return;
    }
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

usize hkDList_TYPE_get_length(hkDList_TYPE *list) {
    return list->length;
}

hkBiNode_TYPE *hkDList_TYPE_remove_node(hkDList_TYPE *list, hkBiNode_TYPE *node) {
    list->length -= 1;
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    node->prev = node->next = NULL;
    return node;
}

hkBiNode_TYPE *hkDList_TYPE_remove(hkDList_TYPE *list, TYPE item) {
    hkBiNode_TYPE *result = NULL;
    if (list->length == 0) {
        result = list->head;
        list->head = list->tail = NULL;
        return result;
    }
    hkBiNode_TYPE *iter = list->head;
    for (int i = 0; iter && i < list->length; ++i) {
        if (TYPE_eq(iter->data, item)) {
            result = iter;
            break;
        }
        iter = iter->next;
    }
    if (!iter) {
        return NULL;
    }
    hkDList_TYPE_remove_node(list, iter);
    return result;
}

hkBiNode_TYPE *hkDList_TYPE_remove_at(hkDList_TYPE *list, i32 index) {
    hkBiNode_TYPE *result = hkDList_TYPE_get_at(list, index);
    if (!result) {
        return NULL;
    }
    result = hkDList_TYPE_remove_node(list, result);
    return result;
}

hkBiNode_TYPE *hkDList_TYPE_get_at(hkDList_TYPE *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    hkBiNode_TYPE *iter = list->head;
    for (int i = 0; iter && i < index; ++i) {
        iter = iter->next;
    }
    return iter;
}

void hkDList_TYPE_destroy(hkDList_TYPE **list) {
    hkBiNode_TYPE *iter = (*list)->head;
    while (iter) {
        hkBiNode_TYPE *destroyer = iter;
        iter = iter->next;
        hkBiNode_TYPE_destroy(&destroyer);
    }
    free(*list);
    *list = NULL;
}

void hkDList_TYPE_print(hkDList_TYPE *list) { 
    hkBiNode_TYPE *iter = list->head; 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    printf("\n");
}
#endif
#undef HKDLIST_IMPL
