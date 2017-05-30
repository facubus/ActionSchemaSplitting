
#ifndef __SEQUENTIALIZATION_H
#define __SEQUENTIALIZATION_H

#include "SubPartitionList.h"
#include "Partition.h"
#include  "QuotientGraph.h"


typedef struct _Sequentialization{

	SubPartitionList sequence;
	Operator *operator;

} _Sequentialization, *Sequentialization;



Sequentialization Sequentialization_Empty(Operator *op);
Sequentialization Sequentialization_Create(Partition p, QuotientGraph *g);

int Sequentialization_Size(const Sequentialization seq);

SubPartition Sequentialization_GetPosition(const Sequentialization seq, int index); /* 1<=index<=size*/
SubPartition Sequentialization_GetFirst(const Sequentialization seq);
SubPartition Sequentialization_GetLast(const Sequentialization seq);

Bool Sequentialization_isSingleton(const Sequentialization seq);

void Sequentialization_AddSubPartition(Sequentialization seq, SubPartition sp);
void Sequentialization_AddSubPartitionList(Sequentialization seq, SubPartitionList *list);


Bool Sequentialization_Before(Sequentialization seq, int var, int position);
Bool Sequentialization_After(Sequentialization seq, int var, int position);


void Sequentialization_Print(Sequentialization seq);



/* decoration*/
void Sequentialization_PutTokenProcnone(Sequentialization seq);
void Sequentialization_PutTokenDo(Sequentialization seq);
void Sequentialization_PutTokenArg(Sequentialization seq);
void Sequentialization_ClearTokens(Sequentialization seq);
void Sequentialization_PutTokens(Sequentialization seq);




#endif
