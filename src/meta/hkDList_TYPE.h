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
