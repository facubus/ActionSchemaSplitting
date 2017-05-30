#include "ff.h"
#include "inst_pre.h"
#include "BlackBoxList.h"


BlackBoxList BlackBoxList_Create(){
	BlackBoxList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}


void BlackBoxList_Add(BlackBoxList *l, BlackBox x){
	bb_node *n = (bb_node *)malloc (sizeof(bb_node));
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


Bool BlackBoxList_IsEmpty(const BlackBoxList *l){
	return l->size == 0;
}


int BlackBoxList_Size(const BlackBoxList *l){
	return l->size;
}


BlackBox BlackBoxList_Get(const BlackBoxList *l, const int index){
	bb_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}

void BlackBoxList_Print(const BlackBoxList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		bb_node *current = l->first;
		while (current != NULL){
			BlackBox_Print(current->info);
			current= current->next;
		}
		printf("\n");

	}
}

