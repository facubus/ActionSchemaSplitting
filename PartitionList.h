
#ifndef __PARTITIONLIST_H
#define __PARTITIONLIST_H

#include "Partition.h"

typedef struct _p_node {
	Partition info;
	struct _p_node *next;
}p_node;


typedef struct _PartitionList {
	p_node *first;
	p_node *last;
	int size;
} PartitionList;




PartitionList PartitionList_Create();
void PartitionList_Add(PartitionList *l, Partition p);
void Partition_List_AddPartitionList(PartitionList *l, PartitionList *r);

Bool PartitionList_IsEmpty(const PartitionList *l);
int PartitionList_Size(const PartitionList *l);
Partition PartitionList_Get(const PartitionList *l, const int index); /* 1 <= index <= size*/
void PartitionList_Print(const PartitionList *l);


int PartitionList_Minimum(const PartitionList *l, IntegerList *indexes);
PartitionList PartitionList_Sort(const PartitionList *l);


void PartitionList_LevelSeqTradeoff(PartitionList *l,Operator *op, int level, float gamma, int atom_partition_size);


void PartitionList_Set(PartitionList *l, Partition p, int index); /* 1 <= index <= size */
void PartitionList_Swap(PartitionList *l, int i, int j); /* 1 <= i,j <= size */



int PartitionList_GetMin(const PartitionList *l); /*return the index with min trade-off value*/
Partition PartitionList_GetMinPartition(const PartitionList *l); /*return the partition min trade-off value*/


void PartitionList_Main();

#endif
