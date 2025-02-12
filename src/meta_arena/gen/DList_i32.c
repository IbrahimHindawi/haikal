#include <core.h>

#include "DList_i32.h"

DList_i32 *DList_i32_create(Arena *arena) {
    DList_i32 *list = arenaPushStruct(arena, DList_i32);
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void DList_i32_insert_at(Arena *arena, DList_i32 *list, i32 item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    }
    if (list->length == index) {
        DList_i32_append(arena, list, item);
    } else if (index == 0) {
        DList_i32_prepend(arena, list, item);
    }
    list->length += 1;
    BiNode_i32 *iter = DList_i32_get_at(arena, list, index);
    BiNode_i32 *node = BiNode_i32_create(arena, item);
    node->next = iter;
    node->prev = iter->prev;
    iter->prev = node;
    if (iter->prev) {
        iter->prev->next = iter;
    }
}

void DList_i32_append(Arena *arena, DList_i32 *list, i32 item) {
    if (!list->head) {
        list->length += 1;
        list->head = BiNode_i32_create(arena, item);
        list->tail = list->head;
        return;
    }
    if (!list->tail) {
        list->length += 1;
        list->tail = BiNode_i32_create(arena, item);
        list->head->next = list->tail;
        return;
    }
    list->length += 1;
    BiNode_i32 *node = BiNode_i32_create(arena, item);
    node->prev = list->tail;
    list->tail->next = node; 
    list->tail = node;
}

void DList_i32_prepend(Arena *arena, DList_i32 *list, i32 item) {
    BiNode_i32 *node = BiNode_i32_create(arena, item);
    list->length += 1;
    if (!list->head) {
        list->head = list->tail = node;
        return;
    }
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

usize DList_i32_get_length(Arena *arena, DList_i32 *list) {
    return list->length;
}

BiNode_i32 *DList_i32_remove_node(Arena *arena, DList_i32 *list, BiNode_i32 *node) {
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

BiNode_i32 *DList_i32_remove(Arena *arena, DList_i32 *list, i32 item) {
    BiNode_i32 *result = NULL;
    if (list->length == 0) {
        result = list->head;
        list->head = list->tail = NULL;
        return result;
    }
    BiNode_i32 *iter = list->head;
    for (int i = 0; iter && i < list->length; ++i) {
        if (i32_eq(iter->data, item)) {
            result = iter;
            break;
        }
        iter = iter->next;
    }
    if (!iter) {
        return NULL;
    }
    DList_i32_remove_node(arena, list, iter);
    return result;
}

BiNode_i32 *DList_i32_remove_at(Arena *arena, DList_i32 *list, i32 index) {
    BiNode_i32 *result = DList_i32_get_at(arena, list, index);
    if (!result) {
        return NULL;
    }
    result = DList_i32_remove_node(arena, list, result);
    return result;
}

BiNode_i32 *DList_i32_get_at(Arena *arena, DList_i32 *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    BiNode_i32 *iter = list->head;
    for (int i = 0; iter && i < index; ++i) {
        iter = iter->next;
    }
    return iter;
}

void DList_i32_destroy(Arena *arena, DList_i32 **list) {
    BiNode_i32 *iter = (*list)->head;
    while (iter) {
        BiNode_i32 *destroyer = iter;
        iter = iter->next;
        BiNode_i32_destroy(arena, &destroyer);
    }
    // free(arena, *list);
    *list = NULL;
}

void DList_i32_print(Arena *arena, DList_i32 *list) { 
    BiNode_i32 *iter = list->head; 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
    printf("\n");
}

