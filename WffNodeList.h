
#ifndef __WFFNODELIST_H
#define __WFFNODELIST_H

#include "ff.h"

typedef struct _wffnode_node {
	WffNode * info;
	struct _wffnode_node *next;
}wffnode_node;


typedef struct _WffNodeList {
	wffnode_node *first;
	wffnode_node *last;
	int size;
}WffNodeList;




WffNodeList WffNodeList_Create();
void WffNodeList_Add(WffNodeList *l, WffNode *x);
Bool WffNodeList_IsEmpty(const WffNodeList *l);
int WffNodeList_Size(const WffNodeList *l);
WffNode* WffNodeList_Get(const WffNodeList *l, const int index); /* 1 <= index <= size*/
void WffNodeList_Print(const WffNodeList *l);



#endif
