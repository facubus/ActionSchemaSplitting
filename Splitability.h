
#ifndef __SPLITABILITY_H
#define __SPLITABILITY_H

#include "ff.h"
#include "inst_pre.h"
#include "IntegerList.h"
#include "FloatList.h"
#include "InterfaceGraph.h"


typedef struct _Splitability{
	FloatList splitability_coef; /*save the splitability coefficients (sorted)*/
	IntegerList operators_index; /*save operators'indexes sorted by splitability value */

	float splitabity_domain;

	float splitability_avg;
	int amount_accepted_operators;

	/*normalization constants*/
	float maxDensity;
	int maxSize;
	int maxCC;
	float maxGrounded;



} _Splitability, *Splitability;



Splitability Splitability_Create(char *visual_graph_file);
int Splitability_GetSize(Splitability splitability);
int Splitability_AmountAcceptedOperators(Splitability splitability); /*return the number of operator below of the splitability average */
int Splitability_GetOperatorIndex(Splitability splitability, int rank); /* 1<= rank <= Splitability_GetSize()*/
Operator* Splitability_GetOperator(Splitability splitability, int rank); /* 1<= rank <= Splitability_GetSize()*/
float Splitability_GetCoefficient(Splitability splitability, int rank); /* 1<= rank <= Splitability_GetSize()*/
int Splitability_GetRank(Splitability splitability, int operator_index); /* 1<= operator_index <= Splitability_GetSize()*/


float Splitability_ComputeCoefficient(Splitability splitability,InterfaceGraph g); /*splitability coefficient in [0,1]*/




void Splitability_Print(Splitability splitability);
void Splitability_Main(char *visual_graph_file);


#endif
