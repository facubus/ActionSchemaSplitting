
#ifndef __SUBPARTITIONLIST_H
#define __SUBPARTITIONLIST_H

#include "SubPartition.h"

typedef struct _sp_node {
	SubPartition info;
	struct _sp_node *next;
}sp_node;


typedef struct _SubPartitionList {
	sp_node *first;
	sp_node *last;
	int size;
}SubPartitionList;




SubPartitionList SubPartitionList_Create();
void SubPartitionList_Add(SubPartitionList *l, SubPartition sp);
Bool SubPartitionList_IsEmpty(const SubPartitionList *l);
int SubPartitionList_Size(const SubPartitionList *l);
SubPartition SubPartitionList_Get(const SubPartitionList *l, const int index); /* 1 <= index <= size*/
void SubPartitionList_Print(const SubPartitionList *l);


int SubPartitionList_Maximum(const SubPartitionList *l, IntegerList *except);
SubPartitionList SubPartitionList_Sort(const SubPartitionList *l);


/*
Bool SubPartitionList_Contains(const SubPartitionList *l, const SubPartition sp);
*/


#endif
