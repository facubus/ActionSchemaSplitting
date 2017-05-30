#include "ff.h"
#include "inst_pre.h"
#include "HeuristicMeasureList.h"


HeuristicMeasureList HeuristicMeasureList_Create(){
	HeuristicMeasureList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}



void HeuristicMeasureList_Add(HeuristicMeasureList *l, HeuristicMeasure x){
	hm_node *n = (hm_node *)malloc(sizeof(hm_node));
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


int HeuristicMeasureList_Size(const HeuristicMeasureList *l){
	return l->size;
}


HeuristicMeasure HeuristicMeasureList_Get(const HeuristicMeasureList *l, const int index){
	hm_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}


HeuristicMeasure HeuristicMeasureList_GetLast(const HeuristicMeasureList *l){
	return HeuristicMeasureList_Get(l, HeuristicMeasureList_Size(l));
}


void HeuristicMeasureList_Print(const HeuristicMeasureList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		hm_node *current = l->first;
		while (current != NULL){
			HeuristicMeasure_Print(current->info);
			current= current->next;
		}
		printf("\n");
	}
}

