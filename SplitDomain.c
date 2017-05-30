#include "ff.h"
#include "inst_pre.h"
#include "SplitDomain.h"

#include "ServicesDomain.h"
#include "ServicesAction.h"
#include "Splitability.h"
#include "Groundability.h"




SplitDomain SplitDomain_Empty(){
	SplitDomain split_domain = (SplitDomain) malloc (sizeof(_SplitDomain));
	split_domain->splt = SplitActionList_Create();
	split_domain->splitability = NULL;
	split_domain->groundability = NULL;
	split_domain->splitted_opearators = IntegerList_Create();
	return split_domain;
}


SplitDomain SplitDomain_Create(char *visual_graph_file, float gamma,int b, int threshold){

	SplitDomain split_domain = SplitDomain_Empty();
	Splitability splitability = Splitability_Create(visual_graph_file);
	Splitability_Print(splitability);


	int i,n = ServicesDomain_GetAmountActions();
	for(i = 1; i <= n; i++){
		Operator* op = ServicesDomain_GetAction(i);
		BlackBoxAction a = BlackBoxAction_Create(op);
		printf("Optimizing operator %s ...\n", a->op->name);

		Partition p;
		int rank = Splitability_GetRank(splitability,i);

		if (rank <= threshold){
			p = Partition_HillClimbingPartition(a,gamma);
		}else{
			/*printf("TrivialSplit: %s InterfaceSize  = %d \n", a->op->name, ServicesAction_GetInterfaceSize(op));*/
			p = Partition_TrivialPartition(a);
		}

		SplitActionList_Add(&split_domain->splt, SplitAction_Create_From_Partition(p));
	}

	return split_domain;

	/* under groundability
	SplitDomain split_domain = SplitDomain_Empty();
	split_domain->splitability = Splitability_Create(visual_graph_file);
	Splitability_Print(split_domain->splitability);

	split_domain->groundability = Groundability_Create(split_domain->splitability,gamma);
	Groundability_Print(split_domain->groundability);

	int i,n = ServicesDomain_GetAmountActions();
	for(i = 1; i <= n; i++){
		if (Groundability_Contains(split_domain->groundability,i)){
			int rank = Groundability_GetRank(split_domain->groundability,i);
			SplitActionList_Add(&split_domain->splt, SplitAction_Create_From_Partition(Groundability_GetPartition(split_domain->groundability,rank)));
		}else{
			BlackBoxAction a = BlackBoxAction_Create(ServicesDomain_GetAction(i));
			Partition trivial_partition = Partition_TrivialPartition(a);
			SplitActionList_Add(&split_domain->splt, SplitAction_Create_From_Partition(trivial_partition));
		}
	}
	if (Groundability_WasSplitted(split_domain->groundability,1))
		SplitDomain_AddSplittedOperator(split_domain,Groundability_GetOperatorIndex(split_domain->groundability,1));
	else
		SplitDomain_AddSplittedOperator(split_domain,-1);
	return split_domain;
	*/

}



int SplitDomain_GetSize(const SplitDomain split_domain){
	return SplitActionList_Size(&split_domain->splt);
}

SplitAction SplitDomain_GetSplitAction(const SplitDomain split_domain, int index){
	return SplitActionList_Get(&split_domain->splt,index);
}
void SplitDomain_AddSplitAction(SplitDomain split_domain, SplitAction split_action){
	SplitActionList_Add(&split_domain->splt, split_action);
}

void SplitDomain_AddSplittedOperator(SplitDomain split_domain, int opearator_index){
	IntegerList_Add(&split_domain->splitted_opearators, opearator_index);
}
int SplitDomain_GetSplittedOperator(SplitDomain split_domain, int index){
	return IntegerList_Get(&split_domain->splitted_opearators,index);
}
int SplitDomain_GetSplittedOperatorSize(SplitDomain split_domain){
	return IntegerList_Size(&split_domain->splitted_opearators);
}

void SplitDomain_Print(const SplitDomain split_domain){
	int i,n = SplitDomain_GetSize(split_domain);
	for(i=1;i<=n;i++){
		SplitAction_Print(SplitDomain_GetSplitAction(split_domain,i));
	}
}

