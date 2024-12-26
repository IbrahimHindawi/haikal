#include <core.h>

#include "DList_TYPE.h"

DList_TYPE *DList_TYPE_create() {
    DList_TYPE *list = malloc(sizeof(DList_TYPE));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void DList_TYPE_insert_at(DList_TYPE *list, TYPE item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    }
    if (list->length == index) {
        DList_TYPE_append(list, item);
    } else if (index == 0) {
        DList_TYPE_prepend(list, item);
    }
    list->length += 1;
    BiNode_TYPE *iter = DList_TYPE_get_at(list, index);
    BiNode_TYPE *node = BiNode_TYPE_create(item);
    node->next = iter;
    node->prev = iter->prev;
    iter->prev = node;
    if (iter->prev) {
        iter->prev->next = iter;
    }
}

void DList_TYPE_append(DList_TYPE *list, TYPE item) {
    if (!list->head) {
        list->length += 1;
        list->head = BiNode_TYPE_create(item);
        list->tail = list->head;
        return;
    }
    if (!list->tail) {
        list->length += 1;
        list->tail = BiNode_TYPE_create(item);
        list->head->next = list->tail;
        return;
    }
    list->length += 1;
    BiNode_TYPE *node = BiNode_TYPE_create(item);
    node->prev = list->tail;
    list->tail->next = node; 
    list->tail = node;
}

void DList_TYPE_prepend(DList_TYPE *list, TYPE item) {
    BiNode_TYPE *node = BiNode_TYPE_create(item);
    list->length += 1;
    if (!list->head) {
        list->head = list->tail = node;
        return;
    }
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

usize DList_TYPE_get_length(DList_TYPE *list) {
    return list->length;
}

BiNode_TYPE *DList_TYPE_remove_node(DList_TYPE *list, BiNode_TYPE *node) {
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

BiNode_TYPE *DList_TYPE_remove(DList_TYPE *list, TYPE item) {
    BiNode_TYPE *result = NULL;
    if (list->length == 0) {
        result = list->head;
        list->head = list->tail = NULL;
        return result;
    }
    BiNode_TYPE *iter = list->head;
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
    DList_TYPE_remove_node(list, iter);
    return result;
}

BiNode_TYPE *DList_TYPE_remove_at(DList_TYPE *list, i32 index) {
    BiNode_TYPE *result = DList_TYPE_get_at(list, index);
    if (!result) {
        return NULL;
    }
    result = DList_TYPE_remove_node(list, result);
    return result;
}

BiNode_TYPE *DList_TYPE_get_at(DList_TYPE *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    BiNode_TYPE *iter = list->head;
    for (int i = 0; iter && i < index; ++i) {
        iter = iter->next;
    }
    return iter;
}

void DList_TYPE_destroy(DList_TYPE **list) {
    BiNode_TYPE *iter = (*list)->head;
    while (iter) {
        BiNode_TYPE *destroyer = iter;
        iter = iter->next;
        BiNode_TYPE_destroy(&destroyer);
    }
    free(*list);
    *list = NULL;
}

void DList_TYPE_print(DList_TYPE *list) { 
    BiNode_TYPE *iter = list->head; 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    printf("\n");
}
