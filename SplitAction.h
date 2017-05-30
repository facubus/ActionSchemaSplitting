#ifndef __SPLITACTION_H
#define __SPLITACTION_H



#include "Sequentialization.h"
#include "SubPartition.h"

typedef struct _SplitAction{

	Sequentialization subactions;

} _SplitAction, *SplitAction;



SplitAction SplitAction_Init(Sequentialization seq);
SplitAction SplitAction_Create_From_Operator(Operator * op, float gamma, int b);
SplitAction SplitAction_Create_From_Partition(Partition p);
int SplitAction_AmountSubActions(const SplitAction split_action);
SubPartition SplitAction_GetSubAction(const SplitAction split_action, int index); /*1<=index<=size*/
char* SplitAction_GetName(const SplitAction split_action);
Operator *SplitAction_GetOperator(const SplitAction split_action);

void SplitAction_Print(const SplitAction split_action);



#endif
