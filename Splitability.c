#include "Splitability.h"
#include "ServicesDomain.h"
#include "ServicesAction.h"
#include "InterfaceGraph.h"

Splitability Splitability_Create(char *visual_graph_file){
	Splitability splitability = (Splitability)malloc(sizeof(_Splitability));
	splitability->splitability_coef = FloatList_Create();
	splitability->operators_index = IntegerList_Create();;

	/*set global normalization constants*/
	splitability->maxDensity = ServicesDomain_GetMaxDensity();
	splitability->maxSize = ServicesDomain_GetMaxInterface();
	splitability->maxCC = ServicesDomain_GetMaxCC();
	splitability->maxGrounded = ServicesDomain_GetMaxGrounded();


	/*compute splitability coefficiennt for each operators*/
	int i, n = ServicesDomain_GetAmountActions();
	for (i=1;i<=n;i++){
		Operator*op = ServicesDomain_GetAction(i);
		InterfaceGraph g = InterfaceGraph_Create(op);
		FloatList_Add(&splitability->splitability_coef, Splitability_ComputeCoefficient(splitability,g));
		IntegerList_Add(&splitability->operators_index,i);
	}

	/*sort operators' indexes by splitability values*/
	for (i=1;i<=n;i++){
		int imax = FloatList_Maximum(&splitability->splitability_coef,i);
		FloatList_Swap(&splitability->splitability_coef,i,imax);
		IntegerList_Swap(&splitability->operators_index,i,imax);
	}

	/*compute splitability coefficiennt average and maximum*/
	splitability->splitability_avg = FloatList_Average(&splitability->splitability_coef);
	splitability->splitabity_domain = FloatList_MaximumValue(&splitability->splitability_coef,1);

	/*compute operators'indexes whose splitability coefficient is greater than splitability average*/
	splitability->amount_accepted_operators = 0;
	for (i=1;i<=n;i++){
		if (FloatList_Get(&splitability->splitability_coef,i)>= splitability->splitability_avg)
			splitability->amount_accepted_operators++;
	}

	return splitability;
}


int Splitability_GetSize(Splitability splitability){
	return IntegerList_Size(&splitability->operators_index);
}


int Splitability_AmountAcceptedOperators(Splitability splitability){
	return splitability->amount_accepted_operators;
}


int Splitability_GetOperatorIndex(Splitability splitability,int rank){
	return IntegerList_Get(&splitability->operators_index, rank);
}

Operator* Splitability_GetOperator(Splitability splitability, int rank){
	return ServicesDomain_GetAction(Splitability_GetOperatorIndex(splitability, rank));
}

float Splitability_GetCoefficient(Splitability splitability, int rank){
	return FloatList_Get(&splitability->splitability_coef,rank);
}


int Splitability_GetRank(Splitability splitability, int operator_index){
	return IntegerList_GetIndex(&splitability->operators_index,operator_index);
}




void Splitability_Print(Splitability splitability){
	/*printf("-----------------------------------------------------------------------------------------------\n");*/
	printf("\nSplitability Ranking\n");
	printf("-------------------------------------------------------------------------------------------------\n");
	int i,n = Splitability_GetSize(splitability);
	for (i=1;i<=n;i++){
		int operator_index = Splitability_GetOperatorIndex(splitability,i);
		char* operator_name = ServicesAction_GetName(ServicesDomain_GetAction(operator_index));
		float coefficient =  Splitability_GetCoefficient(splitability,i);
		printf("%d	%s(%d)	%.3f\n", i, operator_name, operator_index, coefficient);

	}
	printf("-------------------------------------------------------------------------------------------------\n");

	/*
	printf("-----------------------------------------------------------------------------------\n");
	printf("Splitability Domain = %f\n", splitability->splitabiity_domain);
	printf("Splitability Average = %f\n", splitability->splitability_avg);
	printf("-----------------------------------------------------------------------------------\n");

	printf("Splitability Ranking:\n");
	n = Splitability_AmountAcceptedOperators(splitability);
	for (i=1;i<=n;i++){
		int operator_index = Splitability_GetOperatorIndex(splitability,i);
		char* operator_name = ServicesAction_GetName(ServicesDomain_GetAction(operator_index));
		float coefficient =  Splitability_GetCoefficient(splitability,i);
		printf("%d %s(%d)	%f\n", i, operator_name, operator_index, coefficient);
	}
	printf("-----------------------------------------------------------------------------------\n");
	*/
}





float Splitability_ComputeCoefficient(Splitability splitability, InterfaceGraph g){
	if (g->num_nodes < 2){
		return 0.0;
	}else{

		/* Local Normalization*/
		/*
		float local_normalized_density = InterfaceGraph_Density(g);
		float local_normalized_size = 1.0 / InterfaceGraph_Size(g);
		float local_normalized_cc = (float)InterfaceGraph_StronglyConnectedComponents(g)/ g->num_nodes;
		 */

		/* Global Normalization */
		float global_normalized_density = InterfaceGraph_Density(g) / splitability->maxDensity;
		float global_normalized_size = (float)InterfaceGraph_Size(g) / splitability->maxSize;
		float global_normalized_cc = (float)InterfaceGraph_StronglyConnectedComponents(g)/ splitability->maxCC;
		float global_normalized_grounded = ServicesAction_GetGrounded(g->op) /splitability->maxGrounded;



		/*float  splitability_coefficient_1 = ((1.0 - local_normalized_density + 1.0 - local_normalized_size + local_normalized_cc) - 1.0) / 2.0;*/
		/*float  splitability_coefficient_2 = (1.0 - global_normalized_density + global_normalized_size + global_normalized_cc) / 3.0;*/
		float  splitability_coefficient_3 = (1.0 - global_normalized_density + global_normalized_size + global_normalized_cc + global_normalized_grounded) / 4.0;


		/*printf("%s	%.3f\n", g->op->name, InterfaceGraph_Density(g));*/
		/*printf("%s	%f	%f	%f	%f\n", g->op->name, 1.0 - global_normalized_density,global_normalized_size,global_normalized_cc, splitability_coefficient_2);*/
		return splitability_coefficient_3;
	}





}

void Splitability_Main(char *visual_graph_file){
	Splitability s = Splitability_Create(visual_graph_file);
	Splitability_Print(s);
}
