#include "ff.h"
#include "inst_pre.h"
#include "SplitAction.h"
#include "ServicesDomain.h"


SplitAction SplitAction_Init(Sequentialization seq){
	SplitAction split_action = (SplitAction) malloc (sizeof(_SplitAction));
	split_action->subactions = seq;
	return split_action;

}

SplitAction SplitAction_Create_From_Operator(Operator *op, float gamma, int b){
	BlackBoxAction a = BlackBoxAction_Create(op);
	Partition p = Partition_HillClimbingPartition(a,gamma); 	/*!!!PARTITION CALL!!!*/
	QuotientGraph g = QuotientGraph_Create(p);
	Sequentialization seq = Sequentialization_Create(p,&g);
	Sequentialization_PutTokens(seq);
	return SplitAction_Init(seq);
}


SplitAction SplitAction_Create_From_Partition(Partition p){
	QuotientGraph g = QuotientGraph_Create(p);
	Sequentialization seq = Sequentialization_Create(p,&g);
	Sequentialization_PutTokens(seq);
	return SplitAction_Init(seq);
}




int SplitAction_AmountSubActions(const SplitAction split_action){
	return Sequentialization_Size(split_action->subactions);
}
SubPartition SplitAction_GetSubAction(const SplitAction split_action, int index){
	return Sequentialization_GetPosition(split_action->subactions, index);
}

char* SplitAction_GetName(const SplitAction split_action){
	return split_action->subactions->operator->name;
}

Operator *SplitAction_GetOperator(const SplitAction split_action){
	return split_action->subactions->operator;
}
void SplitAction_Print(const SplitAction split_action){
	Sequentialization_Print(split_action->subactions);
}







