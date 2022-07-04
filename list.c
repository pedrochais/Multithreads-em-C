#include <stdlib.h>
#include "list.h"

#define TRUE 1
#define FALSE 0

SLList *sllCreate()
{
    SLList *list;
    list = (SLList *)malloc(sizeof(SLList));
    if (list != NULL)
    {
        list->first = NULL;
        list->current = NULL;
        return list;
    }
    return NULL;
}

int sllInsertAsLast(SLList *list, void *item)
{
    SLNode *last;
    if (list != NULL)
    {
        SLNode *new_node = (SLNode *)malloc(sizeof(SLNode));
        if (new_node != NULL)
        {
            new_node->data = item;
            new_node->next = NULL;

            if (list->first != NULL)
            {
                list->last->next = new_node;
                list->last = new_node;
            }
            else
            {
                list->first = new_node;
                list->last = list->first;
            }
            
            list->number_of_elements += 1;
            return TRUE;
        }
    }
    return FALSE;
}

void *sllGetFirst(SLList *list)
{
    void *data;
    if (list != NULL)
    {
        if (list->first != NULL)
        {
            data = list->first->data;
            list->current = list->first;
            return data;
        }
    }
    return NULL;
}

void *sllGetNext(SLList *list)
{
    void *data;
    if (list != NULL)
    {
        if (list->current != NULL)
        {
            if (list->current->next != NULL)
            {
                list->current = list->current->next;
                data = list->current->data;
                return data;
            }
        }
    }
    return NULL;
}