#include <limits.h>

#include "ff.h"
#include "inst_pre.h"
#include "FloatList.h"


FloatList FloatList_Create(){
	FloatList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}

void FloatList_Add(FloatList *l, f_elem x){
	f_node *n = (f_node *)malloc (sizeof(f_node));
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


Bool FloatList_IsEmpty(const FloatList *l){
	return l->size == 0;
}


int FloatList_Size(const FloatList *l){
	return l->size;
}


void FloatList_Print(const FloatList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		f_node *current = l->first;
		while (current != NULL){
			printf("%f ", current->info);
			current = current->next;
		}
		printf("\n");


	}
}

f_elem FloatList_Get(const FloatList *l, const int index){
	f_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}


void FloatList_Set(const FloatList *l, const int index, float value){
	f_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	current->info = value;
}



int FloatList_Minimum(const FloatList *l, int k){
	int i, n = FloatList_Size(l);
	int imin = -1;
	float min = INT_MAX;

	for(i=k; i<=n; i++){
		if (FloatList_Get(l,i) < min){
			min = FloatList_Get(l,i);
			imin = i;
		}
	}
	return imin;
}

float FloatList_MinimumValue(const FloatList *l, int k){
	int imin = FloatList_Minimum(l,k);
	return FloatList_Get(l,imin);
}


int FloatList_Maximum(const FloatList *l, int k){
	int i, n = FloatList_Size(l);
	int imax = -1;
	float max = INT_MIN;

	for(i=k; i<=n; i++){
		if (max < FloatList_Get(l,i)){
			max = FloatList_Get(l,i);
			imax = i;
		}
	}
	return imax;
}

float FloatList_MaximumValue(const FloatList *l, int k){
	int imax = FloatList_Maximum(l,k);
	return FloatList_Get(l,imax);
}


void FloatList_Swap(FloatList *l, int i, int j){
	float value_i = FloatList_Get(l,i);
	float value_j = FloatList_Get(l,j);
	FloatList_Set(l,i, value_j);
	FloatList_Set(l,j, value_i);
}



f_elem FloatList_Average(const FloatList *l){
	f_elem avg = 0.0;
	int i, n = FloatList_Size(l);
	for(i=1; i<=n; i++){
		avg += FloatList_Get(l,i);
	}
	return avg/n;

}


















