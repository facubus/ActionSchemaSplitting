#include "ff.h"
#include "inst_pre.h"
#include "BlackBoxActionList.h"


BlackBoxActionList BlackBoxActionList_Create(){
	BlackBoxActionList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}



void BlackBoxActionList_Add(BlackBoxActionList *l, BlackBoxAction black_box_action){
	bba_node *n = (bba_node *)malloc (sizeof(bba_node));
	n->info = black_box_action;
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


Bool BlackBoxActionList_IsEmpty(const BlackBoxActionList *l){
	return l->size == 0;
}


int BlackBoxActionList_Size(const BlackBoxActionList *l){
	return l->size;
}


BlackBoxAction BlackBoxActionList_Get(const BlackBoxActionList *l, const int index){
	bba_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}

void BlackBoxActionList_Print(const BlackBoxActionList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		bba_node *current = l->first;
		while (current != NULL){
			BlackBoxAction_Print(current->info);
			current= current->next;
		}
		printf("\n");

	}
}

