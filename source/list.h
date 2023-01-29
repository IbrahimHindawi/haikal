#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/********************************************************************
 *                                                                  * 
 *  Define a structure for linked list elements.                    *
 *                                                                  *
 *******************************************************************/
typedef struct ListElmt_ {
    void *data;
    struct ListElmt_ *next;
} ListElmt;

/********************************************************************
 *                                                                  * 
 *  Define a structure for linked lists.                            *
 *                                                                  *
 *******************************************************************/
typedef struct List_ {
    int size;

    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    ListElmt *head;
    ListElmt *tail;
} List;

/********************************************************************
 *                                                                  * 
 *----------------------Public Interface----------------------------*
 *                                                                  *
 *******************************************************************/

/*  Initialize List structure */
void list_init(List *list, void  (*destroy)(void *data));

/* frees the linked list elements using the destroy function pointer */
void list_destroy(List *list);

/*  insert an element into the list after the specified element */
/*  passing NULL for element means the element will be inserted at the head */
int list_ins_next(List *list, ListElmt *element, const void *data);

/*  removes an element from the list */
/*  passing NULL for element will remove the element at the head */
int list_rem_next(List *list, ListElmt *element, void **data);

#define list_size(list) ((list)->size)

#define list_head(list) ((list)->head)

#define list_tail(list) ((list)->tail)

#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)

#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)

#define list_data(element) ((element)->data)

#define list_next(element) ((element)->next)

#endif











