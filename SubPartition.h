
#ifndef __SUBPARTITION_H
#define __SUBPARTITION_H

#include "BlackBoxList.h"
#include "SplitTokenList.h"

typedef struct _SubPartition{
	BlackBoxList pre; /*preconditions black boxes*/
	BlackBoxList eff; /*effects black boxes*/

	IntegerList eval;
	IntegerList add;
	IntegerList del;
	IntegerList interface;

	/*tokens system*/
	SplitTokenList token_pre;
	SplitTokenList token_eff;



} _SubPartition, *SubPartition;



SubPartition SubPartition_Empty();
SubPartition SubPartition_Create(BlackBox b);

int SubPartition_GetPreSize(SubPartition sp);
int SubPartition_GetEffSize(SubPartition sp);

BlackBox SubPartition_GetPreBlackBox(SubPartition sp, int index); /*1<=index<=size*/
BlackBox SubPartition_GetEffBlackBox(SubPartition sp, int index); /*1<=index<=size*/

void SubPartition_AddPreBlackBox(SubPartition sp, BlackBox b);
void SubPartition_AddEffBlackBox(SubPartition sp, BlackBox b);



void SubPartition_AddSplitTokenPre(SubPartition sp, SplitToken token);
void SubPartition_AddSplitTokenEff(SubPartition sp, SplitToken token);

SplitToken SubPartition_GetSplitTokenPre(SubPartition sp, int index);  /*1<=index<=size*/
SplitToken SubPartition_GetSplitTokenEff(SubPartition sp, int index); /*1<=index<=size*/

int SubPartition_GetSplitTokenPreSize(SubPartition sp);
int SubPartition_GetSplitTokenEffSize(SubPartition sp);


void SubPartition_ClearSplitTokenPre(SubPartition sp);
void SubPartition_ClearSplitTokenEff(SubPartition sp);





int SubPartition_GetInterfaceSize(const SubPartition sp);
int SubPartition_GetEvalSize(const SubPartition sp);
int SubPartition_GetAddSize(const SubPartition sp);
int SubPartition_GetDelSize(const SubPartition sp);

void SubPartition_SetInterface(SubPartition sp);
void SubPartition_SetEval(SubPartition sp);
void SubPartition_SetAdd(SubPartition sp);
void SubPartition_SetDel(SubPartition sp);
void SubPartition_SetAll(SubPartition sp);


Bool SubPartition_isVar(SubPartition sp , int var);
int SubPartition_GetVar(SubPartition sp, int index); /*1<=index<=size*/

void SubPartition_Print(const SubPartition sp);
void SubPartition_Destroy(const SubPartition sp);

SubPartition SubPartition_Merge(const SubPartition sp1, const SubPartition sp2);
Bool SubPartition_Depends(const SubPartition sp1,const SubPartition sp2);
int SubPartition_TradeOff(const SubPartition sp);


#endif
