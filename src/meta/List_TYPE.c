#include <core.h>

#include "List_TYPE.h"

List_TYPE *List_TYPE_create(void) {
    List_TYPE *list = malloc(sizeof(List_TYPE));
    list->head = NULL;
    list->length = 0;
    return list;
}

void List_TYPE_insert_at(List_TYPE *list, TYPE item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    } else if (list->length == index) {
        List_TYPE_append(list, item);
    } else if (index == 0) {
        List_TYPE_prepend(list, item);
    }
    list->length += 1;
    Node_TYPE *iter = list->head;
    i32 count = 0;
    while (iter) {
        if (count == index) {
            Node_TYPE *next_node = iter->next;
            Node_TYPE *new_node = Node_TYPE_create(item);
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

void List_TYPE_append(List_TYPE *list, TYPE item) {
    if (!list->head) {
        list->head = Node_TYPE_create(item);
        list->length += 1;
        return;
    }
    Node_TYPE *iter = list->head; 
    while (iter->next) { 
        iter = iter->next; 
    }
    iter->next = Node_TYPE_create(item);
    list->length += 1;
    return;
}

void List_TYPE_prepend(List_TYPE *list, TYPE item) {
    Node_TYPE *prepend_node = Node_TYPE_create(item);
    list->length += 1;
    if (!list->head) {
        list->head = prepend_node;
        return;
    }
    prepend_node->next = list->head;
    list->head = prepend_node;
}

usize List_TYPE_get_length(List_TYPE *list) {
    return list->length;
}

Node_TYPE *List_TYPE_remove(List_TYPE *list, TYPE item) {
    if (list->length == 0) {
        return NULL;
    }
    Node_TYPE *result = NULL;
    Node_TYPE *iter = list->head;
    Node_TYPE *prev = NULL;
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

Node_TYPE *List_TYPE_remove_at(List_TYPE *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    Node_TYPE *result = NULL;
    Node_TYPE *iter = list->head;
    Node_TYPE *prev = NULL;
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

Node_TYPE *List_TYPE_get_at(List_TYPE *list, i32 index) {
    if (list->length < index) {
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    Node_TYPE *result = NULL;
    Node_TYPE *iter = list->head;
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

void List_TYPE_destroy(List_TYPE *list) {
    Node_TYPE *iter = list->head;
    while (iter) {
        Node_TYPE *destroyer = iter;
        iter = iter->next;
        Node_TYPE_destroy(&destroyer);
    }
    // free(*list);
    list->head = NULL;
    list->length = 0;
}

void List_TYPE_print(List_TYPE *list) { 
    Node_TYPE *iter = list->head; 
    printf("list.length: %llu\n", list->length); 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
}
