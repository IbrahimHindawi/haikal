#pragma once
/* hkList_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkNode_TYPE.h"

structdef(hkList_TYPE) {
    hkNode_TYPE *head;
    usize length;
};

hkList_TYPE *hkList_TYPE_create(void);
void hkList_TYPE_insert_at(hkList_TYPE *list, TYPE item, i32 index);
void hkList_TYPE_append(hkList_TYPE *list, TYPE item);
void hkList_TYPE_prepend(hkList_TYPE *list, TYPE item);
usize hkList_TYPE_get_length(hkList_TYPE *list);
hkNode_TYPE *hkList_TYPE_remove(hkList_TYPE *list, TYPE item);
hkNode_TYPE *hkList_TYPE_remove_at(hkList_TYPE *list, i32 index);
hkNode_TYPE *hkList_TYPE_get_at(hkList_TYPE *list, i32 index);
void hkList_TYPE_destroy(hkList_TYPE *list);
void hkList_TYPE_print(hkList_TYPE *list);
