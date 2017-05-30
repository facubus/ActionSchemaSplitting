#include "ff.h"
#include "inst_pre.h"
#include "SubPartition.h"




SubPartition SubPartition_Empty(){
	SubPartition sp = (SubPartition) malloc (sizeof(_SubPartition));
	sp->pre = BlackBoxList_Create();
	sp->eff = BlackBoxList_Create();

	sp->eval = IntegerList_Create();
	sp->add = IntegerList_Create();
	sp->del = IntegerList_Create();
	sp->interface = IntegerList_Create();

	sp->token_pre = SplitTokenList_Create();
	sp->token_eff = SplitTokenList_Create();

	return sp;
}


SubPartition SubPartition_Create(BlackBox b){
	SubPartition sp = SubPartition_Empty();

	if (BlackBox_IsFormula(b))
		BlackBoxList_Add(&sp->pre,b);
	else if(BlackBox_IsEffFormula(b))
		BlackBoxList_Add(&sp->eff,b);

	SubPartition_SetAll(sp);

	return sp;
}



int SubPartition_GetPreSize(SubPartition sp){
	return BlackBoxList_Size(&sp->pre);
}
int SubPartition_GetEffSize(SubPartition sp){
	return BlackBoxList_Size(&sp->eff);
}


BlackBox SubPartition_GetPreBlackBox(SubPartition sp, int index){
	return BlackBoxList_Get(&sp->pre,index);
}
BlackBox SubPartition_GetEffBlackBox(SubPartition sp, int index){
	return BlackBoxList_Get(&sp->eff,index);
}


void SubPartition_AddPreBlackBox(SubPartition sp, BlackBox b){
	BlackBoxList_Add(&sp->pre, b);
}
void SubPartition_AddEffBlackBox(SubPartition sp, BlackBox b){
	BlackBoxList_Add(&sp->eff, b);
}


void SubPartition_AddSplitTokenPre(SubPartition sp, SplitToken token){
	SplitTokenList_Add(&sp->token_pre, token);
}
void SubPartition_AddSplitTokenEff(SubPartition sp, SplitToken token){
	SplitTokenList_Add(&sp->token_eff, token);
}
SplitToken SubPartition_GetSplitTokenPre(SubPartition sp, int index){
	return SplitTokenList_Get(&sp->token_pre, index);
}

SplitToken SubPartition_GetSplitTokenEff(SubPartition sp, int index){
	return SplitTokenList_Get(&sp->token_eff, index);
}

int SubPartition_GetSplitTokenPreSize(SubPartition sp){
	return SplitTokenList_Size(&sp->token_pre);
}

int SubPartition_GetSplitTokenEffSize(SubPartition sp){
	return SplitTokenList_Size(&sp->token_eff);

}


void SubPartition_ClearSplitTokenPre(SubPartition sp){
	SplitTokenList_ClearAll(&sp->token_pre);
}
void SubPartition_ClearSplitTokenEff(SubPartition sp){
	SplitTokenList_ClearAll(&sp->token_eff);
}





int SubPartition_GetInterfaceSize(const SubPartition sp){
	return IntegerList_Size(&sp->interface);
}
int SubPartition_GetEvalSize(const SubPartition sp){
	return IntegerList_Size(&sp->eval);
}
int SubPartition_GetAddSize(const SubPartition sp){
	return IntegerList_Size(&sp->add);
}
int SubPartition_GetDelSize(const SubPartition sp){
	return IntegerList_Size(&sp->del);
}


void SubPartition_SetInterface(SubPartition sp){
	/*browse subpartition's preconditions*/
	int i, n = SubPartition_GetPreSize(sp);
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetPreBlackBox(sp,i);
		int j,m = BlackBox_GetAmountFreeVariables(b);
		for(j = 1; j <= m; j++){
			int var = BlackBox_GetFreeVariable(b,j);
			if (!IntegerList_Contains(&sp->interface, var)) IntegerList_Add(&sp->interface, var);
		}
	}

	/*browse subpartition's effects*/
	n = SubPartition_GetEffSize(sp);
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetEffBlackBox(sp,i);
		int j,m = BlackBox_GetAmountFreeVariables(b);
		for(j = 1; j <= m; j++){
			int var = BlackBox_GetFreeVariable(b,j);
			if (!IntegerList_Contains(&sp->interface, var)) IntegerList_Add(&sp->interface, var);
		}
	}

	IntegerList_Sort(&sp->interface);

}
void SubPartition_SetEval(SubPartition sp){
	/*browse subpartition's preconditions*/
	int i,n = SubPartition_GetPreSize(sp);
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetPreBlackBox(sp,i);
		int j,m = BlackBox_GetEvalSize(b);
		for(j = 1; j <= m; j++){
			int atom = BlackBox_GetEval(b,j);
			if (!IntegerList_Contains(&sp->eval, atom)) IntegerList_Add(&sp->eval, atom);
		}
	}

	/*browse subpartition's effects*/
	n = SubPartition_GetEffSize(sp);
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetEffBlackBox(sp,i);
		int j,m = BlackBox_GetEvalSize(b);
		for(j = 1; j <= m; j++){
			int atom = BlackBox_GetEval(b,j);
			if (!IntegerList_Contains(&sp->eval, atom)) IntegerList_Add(&sp->eval, atom);
		}
	}

}

void SubPartition_SetAdd(SubPartition sp){
	/*browse subpartition's effects*/
	int i, n = SubPartition_GetEffSize(sp);
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetEffBlackBox(sp,i);
		int j,m = BlackBox_GetAddSize(b);
		for(j = 1; j <= m; j++){
			int atom = BlackBox_GetAdd(b,j);
			if (!IntegerList_Contains(&sp->add, atom)) IntegerList_Add(&sp->add, atom);
		}
	}
}

void SubPartition_SetDel(SubPartition sp){
	/*browse subpartition's effects*/
	int i,n = SubPartition_GetEffSize(sp);
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetEffBlackBox(sp,i);
		int j,m = BlackBox_GetDelSize(b);
		for(j = 1; j <= m; j++){
			int atom = BlackBox_GetDel(b,j);
			if (!IntegerList_Contains(&sp->del, atom)) IntegerList_Add(&sp->del, atom);
		}
	}
}
void SubPartition_SetAll(SubPartition sp){
	SubPartition_SetInterface(sp);
	SubPartition_SetEval(sp);
	SubPartition_SetAdd(sp);
	SubPartition_SetDel(sp);
}


Bool SubPartition_isVar(SubPartition sp , int var){
	return IntegerList_Contains(&sp->interface,var);
}
int SubPartition_GetVar(SubPartition sp, int index){
	return IntegerList_Get(&sp->interface,index);
}


void SubPartition_Print(const SubPartition sp){
	printf("%s\n", "++++++++ SubPartition ++++++++");
	printf("pre:\n");
	BlackBoxList_Print(&sp->pre);
	printf("eff:\n");
	BlackBoxList_Print(&sp->eff);

}
void SubPartition_Destroy(const SubPartition sp){
	free(sp);
}


SubPartition SubPartition_Merge(const SubPartition sp1, const SubPartition sp2){
/*test ok*/
	SubPartition sp = SubPartition_Empty();
	int i;

	/*browse sp1*/
	for(i = 1; i <= SubPartition_GetPreSize(sp1); i++){
		SubPartition_AddPreBlackBox(sp, SubPartition_GetPreBlackBox(sp1,i));
	}
	for(i = 1; i <= SubPartition_GetEffSize(sp1); i++){
		SubPartition_AddEffBlackBox(sp, SubPartition_GetEffBlackBox(sp1,i));
	}
	/*browse sp2*/
	for(i = 1; i <= SubPartition_GetPreSize(sp2); i++){
		SubPartition_AddPreBlackBox(sp, SubPartition_GetPreBlackBox(sp2,i));
	}
	for(i = 1; i <= SubPartition_GetEffSize(sp2); i++){
		SubPartition_AddEffBlackBox(sp, SubPartition_GetEffBlackBox(sp2,i));
	}

	SubPartition_SetInterface(sp);
	SubPartition_SetEval(sp);
	SubPartition_SetAdd(sp);
	SubPartition_SetDel(sp);

	return sp;
}


Bool SubPartition_Depends(const SubPartition sp1, const SubPartition sp2){
/*test ok*/
	int i,j;
	for(i = 1; i <= SubPartition_GetPreSize(sp1); i++){
		for(j = 1; j <= SubPartition_GetEffSize(sp2); j++){
			if(BlackBox_Depends(SubPartition_GetPreBlackBox(sp1,i), SubPartition_GetEffBlackBox(sp2,j))) return TRUE;
		}
	}

	for(i = 1; i <= SubPartition_GetEffSize(sp1); i++){
		for(j = 1; j <= SubPartition_GetEffSize(sp2); j++){
			if(BlackBox_Depends(SubPartition_GetEffBlackBox(sp1,i), SubPartition_GetEffBlackBox(sp2,j))) return TRUE;
		}
	}

	return FALSE;
}


int SubPartition_TradeOff(const SubPartition sp){
	return SubPartition_GetEvalSize(sp);
}










