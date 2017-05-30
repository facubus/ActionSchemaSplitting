#include "ff.h"
#include "inst_pre.h"
#include "PartitionList.h"
#include "ServicesDomain.h"
#include "Partition.h"

PartitionList PartitionList_Create(){
	PartitionList l;
	l.first = NULL;
	l.last = NULL;
	l.size = 0;
	return l;
}



void PartitionList_Add(PartitionList *l, Partition p){
	p_node *n = (p_node *)malloc (sizeof(p_node));
	n->info = p;
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


void Partition_List_AddPartitionList(PartitionList *l, PartitionList *r){
	int i,n = PartitionList_Size(r);
	for(i=1;i<=n;i++){
		PartitionList_Add(l, PartitionList_Get(r,i));
	}
}




Bool PartitionList_IsEmpty(const PartitionList *l){
	return l->size == 0;
}


int PartitionList_Size(const PartitionList *l){
	return l->size;
}


Partition PartitionList_Get(const PartitionList *l, const int index){
	p_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	return current->info;
}


void PartitionList_Print(const PartitionList *l){
	if (l->size == 0) {
		printf("%s\n", "<empty>");
	}else{
		p_node *current = l->first;
		while (current != NULL){
			Partition_Print(current->info);
			current= current->next;
		}
		printf("\n");

	}
}







int PartitionList_Minimum(const PartitionList *l, IntegerList *indexes){
	int i,n = PartitionList_Size(l);
	float min = 1.1;
	int index = -1;
	for(i = 1; i <= n; i++){
		if (!IntegerList_Contains(indexes, i)){
			float tradeOff = PartitionList_Get(l,i)->tradeoff;
			if (tradeOff < min){
				min = tradeOff;
				index = i;
			}
		}
	}
	return index;

}


PartitionList PartitionList_Sort(const PartitionList *l){
	/* sort the list by partition tradeoff*/
	int n = PartitionList_Size(l);
	IntegerList indexes = IntegerList_Create();
	int i=1;
	while(i <= n){
		IntegerList_Add(&indexes, PartitionList_Minimum(l,&indexes));
		i++;
	}

	PartitionList sortlist = PartitionList_Create();
	for(i = 1; i <= n; i++){
		PartitionList_Add(&sortlist, PartitionList_Get(l, IntegerList_Get(&indexes,i)));

	}
	return sortlist;
}



void PartitionList_LevelSeqTradeoff(PartitionList *l, Operator *op, int level, float gamma, int atom_partition_size){
	int i, n = PartitionList_Size(l);
	for(i = 1; i <= n; i++){
		Partition suc = PartitionList_Get(l,i);
		suc->op = op;
		suc->level = level;
		suc->seq = i;
		suc->tradeoff = Partition_TradeOff(suc,gamma,atom_partition_size);
	}
}


void PartitionList_Set(PartitionList *l, Partition p, int index){
	p_node *current = l->first;
	int j=1;
	while (j<index){
		current = current->next;
		j++;
	}
	current->info = p;
}


void PartitionList_Swap(PartitionList *l, int i, int j){
	Partition value_i = PartitionList_Get(l,i);
	Partition value_j = PartitionList_Get(l,j);
	PartitionList_Set(l,value_i,j);
	PartitionList_Set(l,value_j,i);
}



int PartitionList_GetMin(const PartitionList *l){
	int i,n = PartitionList_Size(l);
	int i_min = 0;
	float v_min = 1.1;
	for(i=1;i<=n;i++){
		Partition p = PartitionList_Get(l,i);
		if (p->tradeoff < v_min ){
			i_min = i;
			v_min = p->tradeoff;
		}
	}
	return i_min;
}


Partition PartitionList_GetMinPartition(const PartitionList *l){
	int i_min = PartitionList_GetMin(l);
	Partition p_min = PartitionList_Get(l,i_min);;
	return p_min;
}




void PartitionList_Main(){
	Operator *op = ServicesDomain_GetAction(1);
	BlackBoxAction a = BlackBoxAction_Create(op);
	Partition p = Partition_AtomPartition(a,0.0);
	Partition q = Partition_UnSplitPartition(a,0.0,Partition_GetSize(p));

	PartitionList  l = PartitionList_Create();
	PartitionList_Add(&l, p);
	PartitionList_Add(&l, q);

	printf("%d\n", Partition_GetSize(PartitionList_Get(&l,1)));
	printf("%d\n", Partition_GetSize(PartitionList_Get(&l,2)));

	PartitionList_Swap(&l,1,2);

	printf("%d\n", Partition_GetSize(PartitionList_Get(&l,1)));
	printf("%d\n", Partition_GetSize(PartitionList_Get(&l,2)));

	/*
	 */

}

