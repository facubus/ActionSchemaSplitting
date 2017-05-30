#include <limits.h>

#include "ff.h"
#include "inst_pre.h"
#include "IntegerList.h"


IntegerList IntegerList_Create(){
	IntegerList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}

void IntegerList_Add(IntegerList *l, elem x){
	node *n = (node *)malloc (sizeof(node));
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


IntegerList IntegerList_Merge(const IntegerList *l1, const IntegerList *l2){
/*test ok*/
	IntegerList merge = IntegerList_Create();
	int i;
	for(i = 1; i <= IntegerList_Size(l1); i++){
		int x = IntegerList_Get(l1, i);
		if (!IntegerList_Contains(&merge, x)){
			IntegerList_Add(&merge, x);
		}
	}
	for(i=1;i<= IntegerList_Size(l2);i++){
		int x = IntegerList_Get(l2, i);
		if (!IntegerList_Contains(&merge, x)){
			IntegerList_Add(&merge, x);
		}
	}
	return merge;
}

Bool IntegerList_IsEmpty(const IntegerList *l){
	return l->size == 0;
}


int IntegerList_Size(const IntegerList *l){
	return l->size;
}

void IntegerList_Print(const IntegerList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		node *current = l->first;
		while (current != NULL){
			printf("%ld ", current->info);
			current= current->next;
		}
		printf("\n");


	}
}

elem IntegerList_Get(const IntegerList *l, const int index){
	node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}


int IntegerList_GetIndex(const IntegerList *l, elem e){
	if (l->size == 0){
		return -1;
	}else{
		node *current = l->first;
		int index = 1;
		do{
			if (current->info == e)
				return index;
			else {
				current = current->next;
				index++;
			}
		}while(current!=NULL);
		return -1;
	}

}



void IntegerList_Set(IntegerList *l, int index, int value){
	node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	current->info = value;
}

Bool IntegerList_Contains(const IntegerList *l, const elem x){
	if (l->size == 0){
		return FALSE;
	}else{
		node *current = l->first;
		do{
			if (current->info == x) return TRUE;
			else current = current->next;
		}while(current!=NULL);
		return FALSE;

	}
}


int IntegerList_Maximum(const IntegerList *l, int k){
	int i, n = IntegerList_Size(l);
	int imax = -1, max = INT_MIN;

	for(i=k; i<=n; i++){
		if (max < IntegerList_Get(l,i)){
			max = IntegerList_Get(l,i);
			imax = i;
		}
	}
	return imax;
}

int IntegerList_Minimum(const IntegerList *l, int k){
	int i, n = IntegerList_Size(l);
	int imin = -1, min = INT_MAX;

	for(i=k; i<=n; i++){
		if (IntegerList_Get(l,i) < min){
			min = IntegerList_Get(l,i);
			imin = i;
		}
	}
	return imin;
}


void IntegerList_Sort(IntegerList *l){
	int k, m = IntegerList_Size(l);
	int imax = -1, max = INT_MIN;

	for(k=1; k<=m; k++){

		imax = IntegerList_Maximum(l,k);
		max = IntegerList_Get(l,imax);

		/*swap*/
		int aux = IntegerList_Get(l,k);
		IntegerList_Set(l,k,max);
		IntegerList_Set(l,imax,aux);

	}


}



void IntegerList_Swap(IntegerList *l, int i, int j){
	int value_i = IntegerList_Get(l,i);
	int value_j = IntegerList_Get(l,j);
	IntegerList_Set(l,i, value_j);
	IntegerList_Set(l,j, value_i);
}













