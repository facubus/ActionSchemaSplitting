
#ifndef __PARTITION_H
#define __PARTITION_H

#include "SubPartitionList.h"
#include "BlackBoxAction.h"
#include "HeuristicMeasureList.h"


typedef struct _Partition{

	SubPartitionList sub_partitions;

	Operator *op;
	int level;
	int seq;
	float tradeoff;
	HeuristicMeasureList measure;




} _Partition, *Partition;



Partition Partition_Create(); /*empty partition */
Partition Partition_AtomPartition(BlackBoxAction a, float gamma);
Partition Partition_UnSplitPartition(BlackBoxAction a, float gamma, int atom_partition_size);
Partition Partition_TrivialPartition(BlackBoxAction a); /*equal to UnsplitPartition() without hill climbing parameters*/
Partition Partition_HillClimbingPartition(BlackBoxAction a, float gamma); /* 0 <= gamma <=1 */


/*Partition Partition_BeamSearchPartition(BlackBoxAction a, float gamma, int b); // 0 <= gamma <=1 & b > 0*/




Partition Partition_Merge(const Partition p, int i, int j); /* 1<= i!=j<=size(p) */
float Partition_TradeOff(const Partition p, float gamma, int atom_partition_size);


void Partition_AddSubPartition(Partition p, const SubPartition sp);
SubPartition Partition_GetSubPartition(const Partition p, const int index); /* 1 <= index <= size*/


int Partition_GetSize(const Partition p);
Bool Partition_isSingleton(const Partition p);

int Partition_GetMaxInterfaceSize(const Partition p);


void Partition_Print(const Partition p);

void Partition_Destroy(const Partition p);

void Partition_AddHeuristicMeasure(Partition p,HeuristicMeasure hm);

double Partition_GetGrounded(Partition p);

void Partition_Main();

#endif
