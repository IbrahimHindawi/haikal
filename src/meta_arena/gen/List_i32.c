#include <core.h>

#include "List_i32.h"

List_i32 *List_i32_create(Arena *arena) {
    List_i32 *list = arenaPushStruct(arena, List_i32);
    list->head = NULL;
    list->length = 0;
    return list;
}

void List_i32_insert_at(Arena *arena, List_i32 *list, i32 item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    } else if (list->length == index) {
        List_i32_append(arena, list, item);
    } else if (index == 0) {
        List_i32_prepend(arena, list, item);
    }
    list->length += 1;
    Node_i32 *iter = list->head;
    i32 count = 0;
    while (iter) {
        if (count == index) {
            Node_i32 *next_node = iter->next;
            Node_i32 *new_node = Node_i32_create(arena, item);
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

void List_i32_append(Arena *arena, List_i32 *list, i32 item) {
    if (!list->head) {
        list->head = Node_i32_create(arena, item);
        list->length += 1;
        return;
    }
    Node_i32 *iter = list->head; 
    while (iter->next) { 
        iter = iter->next; 
    }
    iter->next = Node_i32_create(arena, item);
    list->length += 1;
    return;
}

void List_i32_prepend(Arena *arena, List_i32 *list, i32 item) {
    Node_i32 *prepend_node = Node_i32_create(arena, item);
    list->length += 1;
    if (!list->head) {
        list->head = prepend_node;
        return;
    }
    prepend_node->next = list->head;
    list->head = prepend_node;
}

usize List_i32_get_length(Arena *arena, List_i32 *list) {
    return list->length;
}

Node_i32 *List_i32_remove(Arena *arena, List_i32 *list, i32 item) {
    if (list->length == 0) {
        return NULL;
    }
    Node_i32 *result = NULL;
    Node_i32 *iter = list->head;
    Node_i32 *prev = NULL;
    while (iter) {
        if (i32_eq(iter->data, item)) {
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

Node_i32 *List_i32_remove_at(Arena *arena, List_i32 *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    Node_i32 *result = NULL;
    Node_i32 *iter = list->head;
    Node_i32 *prev = NULL;
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

Node_i32 *List_i32_get_at(Arena *arena, List_i32 *list, i32 index) {
    if (list->length < index) {
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    Node_i32 *result = NULL;
    Node_i32 *iter = list->head;
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

void List_i32_destroy(Arena *arena, List_i32 *list) {
    Node_i32 *iter = list->head;
    while (iter) {
        Node_i32 *destroyer = iter;
        iter = iter->next;
        Node_i32_destroy(arena, &destroyer);
    }
    // free(*list);
    list->head = NULL;
    list->length = 0;
}

void List_i32_print(Arena *arena, List_i32 *list) { 
    Node_i32 *iter = list->head; 
    printf("list.length: %llu\n", list->length); 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
}

