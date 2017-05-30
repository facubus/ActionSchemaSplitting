
#ifndef __SPLITTOKENLIST_H
#define __SPLITTOKENLIST_H

#include "SplitToken.h"

typedef struct st_node {
	SplitToken info;
	struct st_node *next;
}st_node;


typedef struct _SplitTokenList{
	st_node *first;
	st_node *last;
	int size;
}SplitTokenList;




SplitTokenList SplitTokenList_Create();
void SplitTokenList_Add(SplitTokenList *l, SplitToken x);
Bool SplitTokenList_IsEmpty(const SplitTokenList *l);
int SplitTokenList_Size(const SplitTokenList *l);
SplitToken SplitTokenList_Get(const SplitTokenList *l, const int index); /*1<= index<= size*/
void SplitTokenList_Print(const SplitTokenList *l);
void SplitTokenList_ClearLast(SplitTokenList *l); /*delete the last element of the list*/
void SplitTokenList_ClearAll(SplitTokenList *l); /*delete every element of the list*/


#endif
