#include "ff.h"
#include "inst_pre.h"
#include "SplitActionList.h"


SplitActionList SplitActionList_Create(){
	SplitActionList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}


void SplitActionList_Add(SplitActionList *l, SplitAction split_action){
	sa_node *n = (sa_node *)malloc (sizeof(sa_node));
	n->info = split_action;
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


Bool SplitActionList_IsEmpty(const SplitActionList *l){
	return l->size == 0;
}


int SplitActionList_Size(const SplitActionList *l){
	return l->size;
}


SplitAction SplitActionList_Get(const SplitActionList *l, const int index){
	sa_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}

void SplitActionList_Print(const SplitActionList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		sa_node *current = l->first;
		while (current != NULL){
			SplitAction_Print(current->info);
			current= current->next;
		}
		printf("\n");

	}
}

