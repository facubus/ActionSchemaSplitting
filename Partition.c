#include <math.h>

#include "ff.h"
#include "inst_pre.h"
#include "Partition.h"
#include "PartitionList.h"
#include "QuotientGraph.h"
#include "ServicesDomain.h"
#include "ServicesAction.h"
#include "BlackBoxAction.h"

#include "SplitDomain.h"
#include "SplitAction.h"
#include "Sequentialization.h"
#include "SplitOutput.h"


Partition Partition_Create(){
	/*empty partition*/
	Partition p = (Partition) malloc (sizeof(_Partition));
	p->sub_partitions = SubPartitionList_Create();

	p->op = NULL;
	p->level = 0;
	p->seq = 0;
	p->tradeoff = 0.0;
	p->measure = HeuristicMeasureList_Create();

	return p;
}


void Partition_AddSubPartition(Partition p, const SubPartition sp){
	SubPartitionList_Add(&p->sub_partitions, sp);
}

SubPartition Partition_GetSubPartition(const Partition p, const int index){
	return SubPartitionList_Get(&p->sub_partitions, index);
}

int Partition_GetSize(const Partition p){
	return SubPartitionList_Size(&p->sub_partitions);
}

Bool Partition_isSingleton(const Partition p ){
	return Partition_GetSize(p) == 0;

}
int Partition_GetMaxInterfaceSize(const Partition p){

	int i;
	int max = 0;
	for(i = 1 ; i <= Partition_GetSize(p); i++){
		int t = SubPartition_GetInterfaceSize(Partition_GetSubPartition(p, i));
		max = t > max ? t : max;
	}

	return max;
}


Partition Partition_AtomPartition(BlackBoxAction a, float gamma){
	Partition atom_partition = Partition_Create();
	int n = BlackBoxAction_GetPreconditionSize(a);
	int m = BlackBoxAction_GetEffectSize(a);
	int i;
	for(i = 1; i <= n; i++){
		SubPartition sp = SubPartition_Create(BlackBoxAction_GetPrecondition(a, i));
		Partition_AddSubPartition(atom_partition, sp);
	}
	for(i = 1; i <= m; i++){
		SubPartition sp = SubPartition_Create(BlackBoxAction_GetEffect(a, i));
		Partition_AddSubPartition(atom_partition, sp);
	}

	/* kill cycles (if there are) from Atom Partition's Quotient Graph */
	QuotientGraph graph = QuotientGraph_Create(atom_partition);
	if (QuotientGraph_isCyclic(&graph)){
		atom_partition = QuotientGraph_Collapse(atom_partition, &graph);
	}


	atom_partition->op = a->op;
	atom_partition->level = 1;
	atom_partition->seq = 1;

	/*calculate atom partition's trade off by hand*/

	/* option 1
	int N1 = Partition_GetSize(atom_partition);
	unsigned long N2 = N1 * ServicesAction_GetGrounded(atom_partition->op);
	atom_partition->tradeoff =  gamma  + ((1.0-gamma) * Partition_Grounded(atom_partition) / N2);
	 */


	/*option 2*/
	atom_partition->tradeoff = gamma + (1.0-gamma)*(float)Partition_GetMaxInterfaceSize(atom_partition) / a->N2;

	return atom_partition;
}



Partition Partition_UnSplitPartition(BlackBoxAction a, float gamma, int atom_partition_size){
	Partition unsplit_partition = Partition_Create();
	SubPartition sp = SubPartition_Empty();

	int i, n = BlackBoxAction_GetPreconditionSize(a), m = BlackBoxAction_GetEffectSize(a);
	for(i = 1; i <= n; i++){
		SubPartition_AddPreBlackBox(sp,BlackBoxAction_GetPrecondition(a, i));
	}
	for(i = 1; i <= m; i++){
		SubPartition_AddEffBlackBox(sp,BlackBoxAction_GetEffect(a, i));
	}

	SubPartition_SetAll(sp);
	Partition_AddSubPartition(unsplit_partition, sp);

	unsplit_partition->op = a->op;
	unsplit_partition->level = n + m;
	unsplit_partition->seq = 1;
	unsplit_partition->tradeoff = Partition_TradeOff(unsplit_partition,gamma, atom_partition_size);

	return unsplit_partition;

}



Partition Partition_TrivialPartition(BlackBoxAction a){


	Partition trivial_partition = Partition_Create();
	SubPartition sp = SubPartition_Empty();

	int i, n = BlackBoxAction_GetPreconditionSize(a), m = BlackBoxAction_GetEffectSize(a);
	for(i = 1; i <= n; i++){
		SubPartition_AddPreBlackBox(sp,BlackBoxAction_GetPrecondition(a, i));
	}
	for(i = 1; i <= m; i++){
		SubPartition_AddEffBlackBox(sp,BlackBoxAction_GetEffect(a, i));
	}

	SubPartition_SetAll(sp);
	Partition_AddSubPartition(trivial_partition, sp);

	trivial_partition->op = a->op;

	return trivial_partition;



}





Partition Partition_HillClimbingPartition(BlackBoxAction a, float gamma){

	Partition expand = Partition_AtomPartition(a, gamma);
	int atom_partition_size = Partition_GetSize(expand);

	QuotientGraph graph = QuotientGraph_Create(expand);
	Partition hill_climbing = expand;

	PartitionList succesors = QuotientGraph_Succesors(expand, &graph);
	PartitionList_LevelSeqTradeoff(&succesors, a->op,2, gamma, atom_partition_size); /* Set level, seq & tradeoff of the succesors*/

	int level = 2;
	while(!PartitionList_IsEmpty(&succesors)){

		Partition suc_min = PartitionList_GetMinPartition(&succesors);
		if (suc_min->tradeoff <= hill_climbing->tradeoff){
			free(hill_climbing);
			hill_climbing = suc_min;
		}

		expand = suc_min;
		graph = QuotientGraph_Create(expand);

		level++;
		succesors = QuotientGraph_Succesors(expand,&graph);
		PartitionList_LevelSeqTradeoff(&succesors,a->op,level, gamma, atom_partition_size); /*Set level, seq & tradeoff of the succesors*/

		/*printf("%s	Level=%d	tradeoff=%f	Size=%d	Interface=%d \n",a->op->name, level, expand->tradeoff, Partition_GetSize(expand), Partition_GetMaxInterfaceSize(expand));*/
	}

	return hill_climbing;
}




Partition Partition_Merge(const Partition p, int i, int j){ /* 1 <= i!=j <= size(p) */
/*Test OK*/
	Partition q = Partition_Create();
	SubPartition sp = SubPartition_Merge(Partition_GetSubPartition(p,i),Partition_GetSubPartition(p,j));
	Partition_AddSubPartition(q, sp);
	int k;
	for(k = 1; k <= Partition_GetSize(p); k++){
		if (k!=i && k!=j)
			Partition_AddSubPartition(q, Partition_GetSubPartition(p,k));
	}
	return q;
}



/*

Partition Partition_BeamSearchPartition(BlackBoxAction a, float gamma, int b){
	printf("Action %s\n", BlackBoxAction_GetName(a));

	Partition atom_partition = Partition_AtomPartition(a, gamma);
	PartitionList generated_i = PartitionList_Create();
	PartitionList_Add(&generated_i,atom_partition);

	Partition best_partition = atom_partition;

	while(PartitionList_Size(&generated_i) != 0){
		PartitionList sorted_generated_i = PartitionList_Sort(&generated_i); // Sort succesors by partition TradeOff
		printf("%f	%d\n", PartitionList_Get(&sorted_generated_i,1)->tradeoff, Partition_GetSize(PartitionList_Get(&sorted_generated_i,1)));

		best_partition =  (PartitionList_Get(&sorted_generated_i,1)->tradeoff <= best_partition->tradeoff) ? PartitionList_Get(&sorted_generated_i,1): best_partition;

		PartitionList generated_ii = PartitionList_Create();
		int j, n =  PartitionList_Size(&generated_i);
		int m = (b<=n) ? b : n;
		for(j=1; j<=m; j++){
			Partition p = PartitionList_Get(&sorted_generated_i, j);
			QuotientGraph graph = QuotientGraph_Create(p);
			PartitionList succesors_p = QuotientGraph_Succesors(p, &graph);
			Partition_List_AddPartitionList(&generated_ii, &succesors_p);
			PartitionList_LevelSeqTradeoff(&generated_ii,a->op,2,gamma,a->N1,a->N2); // Set level, seq & tradeoff of the succesors

		}

		generated_i = generated_ii;

	}
	printf("%f	%d (best partition found)\n", best_partition->tradeoff, Partition_GetSize(best_partition));
	return best_partition;
}
*/










float Partition_TradeOff(const Partition p, float gamma, int atom_partition_size){ /* 0 <= gamma <=1*/
	/*option 1
	int N1 = atom_partition_size;
	long N2 = Partition_GetSize(p) * ServicesAction_GetGrounded(p->op);
	return (gamma * Partition_GetSize(p) / N1) + ((1.0-gamma) * Partition_Grounded(p) / N2);
	*/

	/*option 2 */
	int N1 = atom_partition_size;
	int N2 = ServicesAction_GetInterfaceSize(p->op);
	return (gamma * Partition_GetSize(p) / N1) + ((1.0-gamma) * Partition_GetMaxInterfaceSize(p) / N2);



}


void Partition_Print(const Partition p){
	printf("%s\n", "-------- Partition --------");
	SubPartitionList_Print(&p->sub_partitions);
}


void Partition_AddHeuristicMeasure(Partition p, HeuristicMeasure hm){
	HeuristicMeasureList_Add(&p->measure,hm);
}



double Partition_GetGrounded(Partition p){
	int i, k = Partition_GetSize(p);
	double partition_grounded = 0.0;
	for(i = 1; i <= k; i++){
		SubPartition sp = Partition_GetSubPartition(p,i);
		int j,m = SubPartition_GetInterfaceSize(sp);
		double subpartition_grounded = 1.0;
		for(j = 1; j <= m; j++){
			int var = SubPartition_GetVar(sp,j);
			int type = p->op->var_types[-var-1];
			int x = ServicesDomain_GetTypeSize(type);
			double value;
			value = (x == 0) ? 1 : log10(x+10);
			subpartition_grounded *= value;
		}
		partition_grounded += subpartition_grounded;
	}
	return partition_grounded;
}


void Partition_Main(){

	/*
	Operator *op = ServicesDomain_GetAction(1);
	BlackBoxAction a = BlackBoxAction_Create(op);
	Partition p = Partition_AtomPartition(a, 0.0);

	printf("%d\n", Partition_Grounded(p));
	Partition_Print(p);
	Partition_Grounded(p);
	*/
}



