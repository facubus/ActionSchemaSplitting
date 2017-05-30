#ifndef __BLACKBOXACTIONLIST_H
#define __BLACKBOXACTIONLIST_H


#include "BlackBoxAction.h"

typedef struct bba_node {
	BlackBoxAction info;
	struct bba_node *next;
}bba_node;


typedef struct _BlackBoxActionList {
	bba_node *first;
	bba_node *last;
	int size;
}BlackBoxActionList;





BlackBoxActionList BlackBoxActionList_Create();
void BlackBoxActionList_Add(BlackBoxActionList *l, BlackBoxAction black_box_action);
Bool BlackBoxActionList_IsEmpty(const BlackBoxActionList *l);
int BlackBoxActionList_Size(const BlackBoxActionList *l);

BlackBoxAction BlackBoxActionList_Get(const BlackBoxActionList *l, const int index); /* 1 <= index <= size*/
void BlackBoxActionList_Print(const BlackBoxActionList *l);



#endif
