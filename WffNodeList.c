#include "WffNodeList.h"


WffNodeList WffNodeList_Create(){
	WffNodeList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}


void WffNodeList_Add(WffNodeList *l, WffNode *x){
	wffnode_node *n = (wffnode_node *)malloc (sizeof(wffnode_node));
	n->info = x;
	n->next = NULL;

	if(l->size == 0){
		l->first = n;
		l->last = n;
	}else{
		l->last->next = n;
		l->last = n;
	}
	l->size++;
}


Bool WffNodeList_IsEmpty(const WffNodeList *l){
	return l->size == 0;
}


int WffNodeList_Size(const WffNodeList *l){
	return l->size;
}


WffNode* WffNodeList_Get(const WffNodeList *l, const int index){
	wffnode_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}


void WffNodeList_Print(const WffNodeList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		wffnode_node *current = l->first;
		while (current != NULL){
			printf(" %d ", current->info->connective);
			current= current->next;
		}
		printf("\n");

	}
}

