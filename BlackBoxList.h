
#ifndef __BLACKBOXLIST_H
#define __BLACKBOXLIST_H

#include "BlackBox.h"

typedef struct bb_node {
	BlackBox info;
	struct bb_node *next;
}bb_node;


typedef struct _BlackBoxList {
	bb_node *first;
	bb_node *last;
	int size;
}BlackBoxList;




BlackBoxList BlackBoxList_Create();
void BlackBoxList_Add(BlackBoxList *l, BlackBox x);
Bool BlackBoxList_IsEmpty(const BlackBoxList *l);
int BlackBoxList_Size(const BlackBoxList *l);
BlackBox BlackBoxList_Get(const BlackBoxList *l, const int index); /* 1 <= index <= size*/
void BlackBoxList_Print(const BlackBoxList *l);



#endif
