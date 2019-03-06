#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_json_list.h"
#include "simple_json_error.h"

extern int __SJ_DEBUG;

void sj_list_delete(SJList *list)
{
    if (!list)return;
    if (list->elements)
    {
        free(list->elements);
    }
    free(list);
}

SJList *sj_list_new()
{
    return sj_list_new_size(16);
}

SJList *sj_list_new_size(unsigned int count)
{
    SJList *l;
    if (!count)
    {
        sj_set_error("cannot make a list of size zero");
        return NULL;
    }
    l = (SJList *)malloc(sizeof(SJList));
    if (!l)
    {
        sj_set_error("failed to allocate space for the list");
        return NULL;
    }
    memset(l,0,sizeof(SJList));
    l->size = count;
    l->elements = (SJListElementData*)malloc(sizeof(SJListElementData)*count);
    if (!l->elements)
    {
        sj_set_error("failed to allocate space for list elements");
        free(l);
        return NULL;
    }
    memset(l->elements,0,sizeof(SJListElementData)*count);
    return l;
}

void *sj_list_get_nth(SJList *list,unsigned int n)
{
    if (!list)
    {
        sj_set_error("no list provided");
        return NULL;
    }
    if ((n >= list->count)||(n >= list->size))return NULL;
    return list->elements[n].data;
}

SJList *sj_list_expand(SJList *list)
{
    SJList *l;
    if (!list)
    {
        sj_set_error("no list provided");
        return NULL;
    }
    if (!list->size)list->size = 8;
    l = sj_list_new_size(list->size * 2);
    if (!l)
    {
        return list;
    }
    if (list->count > 0)
    {
        memcpy(l->elements,list->elements,sizeof(SJListElementData)*(list->count));
    }
    l->count = list->count;
    sj_list_delete(list);
    return l;
}

SJList *sj_list_append(SJList *list,void *data)
{
    if (!list)
    {
        sj_set_error("no list provided");
        if (__SJ_DEBUG) printf("no list provided\n");
        return NULL;
    }
    if (list->count >= list->size)
    {
        list = sj_list_expand(list);
        if (!list)
        {
            sj_set_error("append failed due to lack of memory");
            if (__SJ_DEBUG) printf("append failed due to lack of memory\n");
            return NULL;
        }
    }
    list->elements[list->count++].data = data;
    return list;
}

SJList *sj_list_prepend(SJList *list,void *data)
{
    return sj_list_insert(list,data,0);
}

SJList *sj_list_insert(SJList *list,void *data,unsigned int n)
{
    if (!list)
    {
        sj_set_error("no list provided");
        return NULL;
    }
    if (n > list->size + 1)
    {
        sj_set_error("attempting to insert element beyond length of list");
        return list;
    }
    if (list->count >= list->size)
    {
        list = sj_list_expand(list);
        if (!list)return NULL;
    }
    memmove(&list->elements[n+1],&list->elements[n],sizeof(SJListElementData)*(list->count - n));//copy all elements after n
    list->elements[n].data = data;
    list->count++;
    return list;
}


SJList *sj_list_delete_first(SJList *list)
{
    return sj_list_delete_nth(list,0);
}

SJList *sj_list_delete_last(SJList *list)
{
    if (!list)
    {
        sj_set_error("no list provided");
        return NULL;
    }
    return sj_list_delete_nth(list,list->count-1);
}

int sj_list_delete_data(SJList *list,void *data)
{
    unsigned int i;
    if (!list)
    {
        sj_set_error("no list provided");
        return -1;
    }
    if (!data)return 0;
    for (i = 0; i < list->count;i++)
    {
        if (list->elements[i].data == data)
        {
            // found it, now delete it
            sj_list_delete_nth(list,i);
            return 0;
        }
    }
    sj_set_error("data not found");
    return -1;
}

SJList *sj_list_delete_nth(SJList *list,unsigned int n)
{
    if (!list)
    {
        sj_set_error("no list provided");
        return NULL;
    }
    if (n >= list->count)
    {
        sj_set_error("attempting to delete beyond the length of the list");
        return list;
    }
    if (n == (list->count - 1))
    {
        list->elements[list->count - 1].data = NULL;
        list->count--;// last element in the list, this is easy
        return list;
    }
    memmove(&list->elements[n],&list->elements[n+1],sizeof(SJListElementData)*(list->count - n));//copy all elements after n
    list->count--;
    return list;
}

unsigned int sj_list_get_count(SJList *list)
{
    if (!list)return 0;
    return list->count;
}

void sj_list_foreach(SJList *list,void (*function)(void *data,void *context),void *contextData)
{
    unsigned int i;
    if (!list)
    {
        sj_set_error("no list provided");
        return;
    }
    if (!function)
    {
        sj_set_error("no function provided");
    }
    for (i = 0;i < list->count;i++)
    {
        function(list->elements[i].data,contextData);
    }
}

/*eol@eof*/
