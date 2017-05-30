#ifndef __SPLITDOMAIN_H
#define __SPLITDOMAIN_H


#include "SplitAction.h"
#include "SplitActionList.h"
#include "Splitability.h"
#include "Groundability.h"



typedef struct _SplitDomain{

	SplitActionList splt;


	Splitability splitability;
	Groundability groundability;
	IntegerList splitted_opearators; /*save the operators were splitted*/


} _SplitDomain, *SplitDomain;


SplitDomain SplitDomain_Empty();
SplitDomain SplitDomain_Create(char *visual_graph_file, float gamma, int b, int threshold);

int SplitDomain_GetSize(const SplitDomain split_domain);
SplitAction SplitDomain_GetSplitAction(const SplitDomain split_domain, int index);/*1<=index<=size*/
void SplitDomain_AddSplitAction(SplitDomain split_domain, SplitAction split_action);

void SplitDomain_AddSplittedOperator(SplitDomain split_domain, int opearator_index);
int SplitDomain_GetSplittedOperator(SplitDomain split_domain, int index); /*1<=index<=size(splitted_operators)*/
int SplitDomain_GetSplittedOperatorSize(SplitDomain split_domain);

void SplitDomain_Print(const SplitDomain split_domain);



#endif
