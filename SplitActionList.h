#ifndef __SPLITACTIONLIST_H
#define __SPLITACTIONLIST_H


#include "SplitAction.h"

typedef struct sa_node {
	SplitAction info;
	struct sa_node *next;
}sa_node;


typedef struct _SplitActionList {
	sa_node *first;
	sa_node *last;
	int size;
}SplitActionList;





SplitActionList SplitActionList_Create();
void SplitActionList_Add(SplitActionList *l, SplitAction split_action);
Bool SplitActionList_IsEmpty(const SplitActionList *l);
int SplitActionList_Size(const SplitActionList *l);

SplitAction SplitActionList_Get(const SplitActionList *l, const int index); /* 1 <= index <= size*/
void SplitActionList_Print(const SplitActionList *l);



#endif
