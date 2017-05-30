#include "ff.h"
#include "inst_pre.h"
#include "SplitTokenList.h"


SplitTokenList SplitTokenList_Create(){
	SplitTokenList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}


void SplitTokenList_Add(SplitTokenList *l, SplitToken x){
	st_node *n = (st_node *)malloc (sizeof(st_node));
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


Bool SplitTokenList_IsEmpty(const SplitTokenList *l){
	return l->size == 0;
}


int SplitTokenList_Size(const SplitTokenList *l){
	return l->size;
}


SplitToken SplitTokenList_Get(const SplitTokenList *l, const int index){
	st_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}

void SplitTokenList_Print(const SplitTokenList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		st_node *current = l->first;
		while (current != NULL){
			SplitToken_Print(current->info);
			current= current->next;
		}
		printf("\n");

	}
}


void SplitTokenList_ClearLast(SplitTokenList *l){
	if (l->size > 0){
		if(l->size == 1){
			free(l->last);
			l->last = NULL;
			l->first = NULL;
		}else{
			st_node *current = l->first;
			int j=1;
			while (j < l->size -1){
				current = current->next;
				j++;
			}
			l->last = current;
			free(current->next);
			current->next = NULL;
		}
		l->size--;
	}
}


void SplitTokenList_ClearAll(SplitTokenList *l){
	while (l->size > 0 ){
		SplitTokenList_ClearLast(l);
	}
}

