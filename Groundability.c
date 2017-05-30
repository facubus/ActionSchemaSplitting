#include "Groundability.h"
#include "BlackBoxAction.h"
#include "Partition.h"
#include "ServicesDomain.h"
#include "ServicesAction.h"

Groundability Groundability_Create(Splitability splitability, float gamma){

	int m = Splitability_AmountAcceptedOperators(splitability);

	Groundability groundability = (Groundability)malloc(sizeof(_Groundability));
	groundability->ranking = IntegerList_Create();
	groundability->delta_grounded = IntegerList_Create();
	groundability->hill_climbing_partition = PartitionList_Create();
	groundability->trivial_partition = PartitionList_Create();


	printf("Grounding ...\n");
	int i;
	for(i = 1; i <= m; i++){
		Operator *op = Splitability_GetOperator(splitability,i);
		BlackBoxAction a = BlackBoxAction_Create(op);
		printf("%d %s\n",i,op->name);

		if (i>2 || i==7 || i==9 || i==18){
			Partition p = Partition_TrivialPartition(a);

			long delta = 0;
			IntegerList_Add(&groundability->delta_grounded, delta);
			PartitionList_Add(&groundability->hill_climbing_partition, p);
			PartitionList_Add(&groundability->trivial_partition,p);

		}else{
			Partition hill_climbing_partition = Partition_HillClimbingPartition(a, gamma); /* !!! PARTITION CALL !!! */

			long delta = 1;
			/*long delta = ServicesAction_GetGrounded(op) - Partition_Grounded(hill_climbing_partition);*/
			IntegerList_Add(&groundability->delta_grounded, delta);
			PartitionList_Add(&groundability->hill_climbing_partition, hill_climbing_partition);
			PartitionList_Add(&groundability->trivial_partition,Partition_TrivialPartition(a));

		}


	}


	/*sort by delta value*/
	IntegerList splitability_copy = IntegerList_Create(); /*aux list*/
	for(i = 1; i <= m; i++){
		IntegerList_Add(&splitability_copy, Splitability_GetOperatorIndex(splitability,i));
	}
	for(i = 1; i <= m; i++){
		int max_index = IntegerList_Maximum(&groundability->delta_grounded, i);
		IntegerList_Add(&groundability->ranking, IntegerList_Get(&splitability_copy ,max_index));
		IntegerList_Swap(&splitability_copy,i,max_index);
		IntegerList_Swap(&groundability->delta_grounded,i,max_index);
		PartitionList_Swap(&groundability->hill_climbing_partition,i,max_index);
		PartitionList_Swap(&groundability->trivial_partition,i,max_index);
	}

	/*
	*/

	return groundability;
}



int Groundability_RankingSize(Groundability groundability){
	return IntegerList_Size(&groundability->ranking);
}

int Groundability_GetOperatorIndex(Groundability groundability, int rank){
	return IntegerList_Get(&groundability->ranking,rank);
}

Partition Groundability_GetPartition(Groundability groundability, int rank){
	if (rank == 1)
		return PartitionList_Get(&groundability->hill_climbing_partition, rank);
	else
		return PartitionList_Get(&groundability->trivial_partition, rank);

}


Bool Groundability_Contains(Groundability groundability, int operator_index){
	return IntegerList_Contains(&groundability->ranking, operator_index);
}

int Groundability_GetRank(Groundability groundability, int operator_index){
	return IntegerList_GetIndex(&groundability->ranking, operator_index);
}


Bool Groundability_WasSplitted(Groundability groundability, int rank){
	Partition p = PartitionList_Get(&groundability->hill_climbing_partition, rank);
	return !Partition_isSingleton(p);
}



void Groundability_Print(Groundability groundability){
	int i, m = Groundability_RankingSize(groundability);
	printf("Groundability Ranking:\n");
	for(i = 1; i <= m; i++){
		int operator_index = Groundability_GetOperatorIndex(groundability,i);
		Operator *op = ServicesDomain_GetAction(operator_index);
		long delta = IntegerList_Get(&groundability->delta_grounded,i);

		Partition hill_climbing_partition =  PartitionList_Get(&groundability->hill_climbing_partition,i);
		long operator_grounded = ServicesAction_GetGrounded(op);
		long partition_grounded = Partition_GetGrounded(hill_climbing_partition);
		int partition_size = Partition_GetSize(hill_climbing_partition);

		printf("%d %s(%d)	HC = <%d, %ld/%ld, %f>	delta = %ld - %ld = %ld\n", i, op->name, operator_index,
				partition_size, partition_grounded, partition_size * operator_grounded, hill_climbing_partition->tradeoff,
				operator_grounded,partition_grounded,delta);
	}

}



void Groundability_Main(float gamma){
	Splitability splitability = Splitability_Create("/home/facundo/Desktop/tempdir/interface_graph.dot");
	Splitability_Print(splitability);
	Groundability groundability = Groundability_Create(splitability,gamma);
	Groundability_Print(groundability);
}
