#include "EffectList.h"


EffectList EffectList_Create(){
	EffectList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}


void EffectList_Add(EffectList *l, Effect *x){
	effect_node *n = (effect_node *)malloc (sizeof(effect_node));
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


Bool EffectList_IsEmpty(const EffectList *l){
	return l->size == 0;
}


int EffectList_Size(const EffectList *l){
	return l->size;
}


Effect* EffectList_Get(const EffectList *l, const int index){
	effect_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}


void EffectList_Print(const EffectList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		effect_node *current = l->first;
		while (current != NULL){
			printf(" %d %d \n", current->info->next == NULL, current->info->prev == NULL );
			current = current->next;
		}
		printf("\n");

	}
}

