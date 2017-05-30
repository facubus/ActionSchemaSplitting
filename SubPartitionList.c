#include "ff.h"
#include "inst_pre.h"
#include "SubPartitionList.h"


SubPartitionList SubPartitionList_Create(){
	SubPartitionList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}





void SubPartitionList_Add(SubPartitionList *l, SubPartition sp){
	sp_node *n = (sp_node *)malloc (sizeof(sp_node));
	n->info = sp;
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



Bool SubPartitionList_IsEmpty(const SubPartitionList *l){
	return l->size == 0;
}


int SubPartitionList_Size(const SubPartitionList *l){
	return l->size;
}



SubPartition SubPartitionList_Get(const SubPartitionList *l, const int index){
	sp_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}



void SubPartitionList_Print(const SubPartitionList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		sp_node *current = l->first;
		while (current != NULL){
			SubPartition_Print(current->info);
			current= current->next;
		}
		printf("\n");

	}
}


int SubPartitionList_Maximum(const SubPartitionList *l, IntegerList *except){
/* return the index imax where l[imax] >= l[i] for all i and imax, i not in except*/

	int i, imax = -1, vmax = -1;
	for(i = 1; i <= SubPartitionList_Size(l); i++){
		if (!IntegerList_Contains(except,i)){
			int v = SubPartition_TradeOff(SubPartitionList_Get(l,i));
			if (v > vmax){
				vmax = v;
				imax = i;
			}
		}
	}
	return imax;
}


SubPartitionList SubPartitionList_Sort(const SubPartitionList *l){
	SubPartitionList sorted_list = SubPartitionList_Create();
	IntegerList except = IntegerList_Create();

	int n = SubPartitionList_Size(l);
	while(SubPartitionList_Size(&sorted_list) < n){
		int imax = SubPartitionList_Maximum(l,&except);
		IntegerList_Add(&except, imax);
		SubPartitionList_Add(&sorted_list, SubPartitionList_Get(l,imax));
	}

	return sorted_list;


}







