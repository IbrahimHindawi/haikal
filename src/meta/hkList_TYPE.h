#pragma once
/* hkList_TYPE */
#include <core.h>

typedef struct hkList_S {
    void *data;
    struct hkList_S *next;
}   hkList_TYPE; 

hkList_TYPE	*hklist_TYPE_new(void *content);
usize	hklist_TYPE_size(hkList_TYPE *lst);
hkList_TYPE	*hklist_TYPE_last(hkList_TYPE *lst);
void	hklist_TYPE_iter(hkList_TYPE *lst, void (*f)(void *));
void	hklist_TYPE_add_front(hkList_TYPE **lst, hkList_TYPE *new);
void	hklist_TYPE_add_back(hkList_TYPE **lst, hkList_TYPE *new);

#ifdef HK_LIST_IMPL
hkList_TYPE	*hklist_TYPE_new(void *content)
{
	hkList_TYPE	*new;

	new = (hkList_TYPE *)malloc(sizeof(hkList_TYPE));
	if (!new)
		return (NULL);
	new->data = content;
	new->next = NULL;
	return (new);
}
usize	hklist_TYPE_size(hkList_TYPE *lst)
{
	usize	res;

	res = 0;
	while (lst)
	{
		lst = lst->next;
		res++;
	}
	return (res);
}
hkList_TYPE	*hklist_TYPE_last(hkList_TYPE *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}
void	hklist_TYPE_iter(hkList_TYPE *lst, void (*f)(void *))
{
	hkList_TYPE	*temp;

	if (!lst || !f)
		return ;
	temp = lst;
	while (temp)
	{
		f(temp->content);
		temp = temp->next;
	}
}
void	hklist_TYPE_add_front(hkList_TYPE **lst, hkList_TYPE *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
void	hklist_TYPE_add_back(hkList_TYPE **lst, hkList_TYPE *new)
{
	hkList_TYPE	*traversep;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	traversep = *lst;
	while (traversep->next)
		traversep = traversep->next;
	traversep->next = new;
}

#endif
