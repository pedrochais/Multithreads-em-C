#ifndef SLLIST_H
#define SLLIST_H

typedef struct _slnode_
{
    void *data;
    struct _slnode_ *next;
} SLNode;

typedef struct _sllist_
{
    SLNode *first;
    SLNode *current;
    SLNode *last;
    int number_of_elements;
} SLList;

/* Aloca memória pra uma estrutura de dados do tipo lista e inicializa ela */
SLList *sllCreate();
/* Insere o próximo nó após o último nó da lista */
int sllInsertAsLast(SLList *list, void *item);
/* Recupera o primeiro nó da lista */
void *sllGetFirst(SLList *list);
/* Recupera o próximo nó da lista */
void *sllGetNext(SLList *list);

#endif
