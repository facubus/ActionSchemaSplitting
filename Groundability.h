
#ifndef __GROUNDABILITY_H
#define __GROUNDABILITY_H

#include "ff.h"
#include "inst_pre.h"
#include "Splitability.h"
#include "PartitionList.h"


typedef struct _Groundability{
	IntegerList ranking; /*a sorted list of index operators (order defined by generated grounded in the partition search algorithm*/
	IntegerList delta_grounded;
	PartitionList hill_climbing_partition;
	PartitionList trivial_partition;


} _Groundability, *Groundability;



Groundability Groundability_Create(Splitability splitability, float gamma);

int Groundability_RankingSize(Groundability groundability);
int Groundability_GetOperatorIndex(Groundability groundability, int rank); /* 1<= rank <= Groundability_RankSize()*/
Partition Groundability_GetPartition(Groundability groundability, int rank); /* 1<= rank <= Groundability_RankSize()*/

Bool Groundability_Contains(Groundability groundability, int operator_index);
int Groundability_GetRank(Groundability groundability, int operator_index);



Bool Groundability_WasSplitted(Groundability groundability, int rank);



void Groundability_Print(Groundability groundability);
void Groundability_Main(float gamma);


#endif
