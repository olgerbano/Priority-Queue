#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef int (*TFComp)(void *a, void *b);
typedef void (*TFreeInfo)(void*);
typedef void (*TFPrintElem)(void*);
typedef struct cell
{
	struct cell *next;
	void *info;
} Node, *TList;

typedef struct
{
	TList l;
	size_t count;
	size_t infoSize;
	TFComp insertComp;
} Queue;

typedef struct
{
	int id;
	int priority;
	int iteration;
} Process;

int compare_process(void *a, void *b)
{
	Process *pa, *pb;
	pa = a;
	pb = b;

	if (pa->priority == pb->priority)
	{
		return pb->iteration - pa->iteration;
	}

	return pa->priority - pb->priority;
}
int list_insert(TList *list, void *info, size_t infoSize)
{
    TList aux = calloc(1, sizeof(Node));
    if (!aux)
    {
        return 0;
    }

    aux->info = calloc(1, infoSize);
    if (!aux->info)
    {
        free(aux);
        return 0;
    }

    memcpy(aux->info, info, infoSize);
    aux->next = *list;
    *list = aux;

    return 1;
}

void list_remove(TList *elem, TFreeInfo freeInfo)
{
    TList aux = *elem;

    if (!aux)
    {
        return;
    }

    freeInfo(aux->info);
    *elem = aux->next;
    free(aux);
}

void list_destroy(TList *list, TFreeInfo freeInfo)
{
    while (*list != NULL)
    {
        list_remove(list, freeInfo);
    }
}

size_t list_length(TList *list)
{
    size_t lg = 0;
    TList p = *list;

    for ( ; p != NULL; p = p->next)
    {
        lg++;
    }

    return lg;
}

void list_print(TList *list, TFPrintElem print_elem)
{
    if (!*list)
    {
        printf("VIDA\n");
        return;
    }

    for ( ; *list != NULL; list = &(*list)->next)
    {
        print_elem((*list)->info);
    }

    printf("\n");
}

Queue* queue_new(size_t infoSize, TFComp insertComp)
{
	Queue *q = calloc(1, sizeof(Queue));
	q->count = 0;
	q->infoSize = infoSize;
	q->l = NULL;
	q->insertComp = insertComp;

	return q;
}
void queue_push(Queue *q, void *info)
{
	TList *l = &(q->l);

	while (*l && q->insertComp((*l)->info, info) > 0)
	{
		l = &(*l)->next;
	}

	TList aux = calloc(1, sizeof(Node));
	aux->info = calloc(1, q->infoSize);
	memcpy(aux->info, info, q->infoSize);

	aux->next = *l;
	*l = aux;

	q->count++;
}
void* queue_pop(Queue *q)
{
	if (q->l == NULL)
	{
		return NULL;
	}

	void *info = q->l->info;

	TList aux = q->l;
	q->l = aux->next;
	free(aux);

	q->count--;

	return info;
}

int queue_isEmpty(Queue *q)
{
	return q->l == NULL;
}

size_t queue_count(Queue *q)
{
	return q->count;
}

void queue_clear(Queue *q, TFreeInfo freeInfo)
{
	list_destroy(&(q->l), freeInfo);
	q->count = 0;
}

void queue_destroy(Queue **q, TFreeInfo freeInfo)
{
	list_destroy(&((*q)->l), freeInfo);
	free(*q);
	*q = NULL;
}
void queue_print(Queue *q, TFPrintElem print_elem)
{
	list_print(&(q->l), print_elem);
}
void print_process(void *a)
{
	Process *p = a;
	printf("%d %d\n", p->id,p->priority);
}
void free_process(void *p)
{
	free(p);
}
int main(){
    Queue *procQ;
    int eventsNr, eventId, procId, priority;
	int i, iteration;
    procQ = queue_new(sizeof(Process), compare_process);
    iteration = 0;
    for(iteration=0;iteration<5;iteration++){
        scanf("%d", &procId);
		scanf("%d", &priority);
        Process p;
		p.id = procId;
		p.priority = priority;
		p.iteration = iteration;
		queue_push(procQ, &p);
        free_process(&p);
    }
    if (!queue_isEmpty(procQ)){
			queue_print(procQ, print_process);
    }
    queue_destroy(&procQ, free_process);

    return 0;
}
