#include "ff.h"
#include "inst_pre.h"
#include "BlackBox.h"
#include "SearchAtoms.h"
#include "SearchFreeVariables.h"



BlackBox BlackBox_Empty(){
	BlackBox b = (BlackBox)malloc (sizeof(_BlackBox));
	b->eval = IntegerList_Create();
	b->add = IntegerList_Create();
	b->del = IntegerList_Create();
	b->free_variables = IntegerList_Create();
	b->formula = NULL;
	b->eff_formula = NULL;
	b->split_token = NULL;
	return b;

}

void BlackBox_AddEval(BlackBox b, int atom){
	IntegerList_Add(&b->eval, atom);
}
void BlackBox_AddAdd(BlackBox b, int atom){
	IntegerList_Add(&b->add, atom);
}
void BlackBox_AddDel(BlackBox b, int atom){
	IntegerList_Add(&b->del, atom);
}
void BlackBox_AddFreeVariable(BlackBox b, int var){
	IntegerList_Add(&b->free_variables, var);
}



BlackBox BlackBox_CreateFromFormula (WffNode *formula, int num_vars){
	BlackBox b = (BlackBox) malloc (sizeof(_BlackBox));

	b->eval = IntegerList_Create();
	b->add = IntegerList_Create();
	b->del = IntegerList_Create();
	SearchAtoms_FromFormula(formula, &(b->eval));

	b->free_variables = IntegerList_Create();
	SearchFreeVariables_FromFormula(formula, num_vars, &b->free_variables);

	b->formula = formula;
	b->eff_formula = NULL;
	b->split_token = NULL;
	return b;
}


BlackBox BlackBox_CreateFromEffFormula(Effect *eff_formula, int num_vars){
	BlackBox b = (BlackBox) malloc (sizeof(_BlackBox));

	b->eval = IntegerList_Create();
	b->add = IntegerList_Create();
	b->del = IntegerList_Create();
	SearchAtoms_FromEffFormula(eff_formula, &(b->eval), &(b->add), &(b->del));

	b->free_variables = IntegerList_Create();
	SearchFreeVariables_FromEffFormula(eff_formula, num_vars, &b->free_variables);


	b->formula = NULL;
	b->eff_formula = eff_formula;
	b->split_token = NULL;
	return b;
}

BlackBox BlackBox_CreateFromSplitToken(SplitToken token){
	BlackBox b;
	b = (BlackBox)malloc (sizeof(_BlackBox));

	b->eval = IntegerList_Create();
	b->add = IntegerList_Create();
	b->del = IntegerList_Create();
	b->free_variables = IntegerList_Create();

	b->formula = NULL;
	b->eff_formula = NULL;
	b->split_token = token;
	return b;

}



int BlackBox_GetEval(const BlackBox b, int index){
	return IntegerList_Get(&b->eval, index);
}
int BlackBox_GetEvalSize(const BlackBox b){
	return IntegerList_Size(&b->eval);
}


int BlackBox_GetAdd(const BlackBox b, int index){
	return IntegerList_Get(&b->add, index);
}
int BlackBox_GetAddSize(const BlackBox b){
	return IntegerList_Size(&b->add);
}

int BlackBox_GetDel(const BlackBox b, int index){
	return IntegerList_Get(&b->del, index);
}
int BlackBox_GetDelSize(const BlackBox b){
	return IntegerList_Size(&b->del);
}



int BlackBox_GetAmountFreeVariables(const BlackBox b){
	return IntegerList_Size(&b->free_variables);
}
int BlackBox_GetFreeVariable(const BlackBox b, const int index){
	return IntegerList_Get(&b->free_variables, index);
}


WffNode *BlackBox_GetFormula(BlackBox b){
	return b->formula;
}
Effect *BlackBox_GetEffFormula(BlackBox b){
	return b->eff_formula;
}
SplitToken BlackBox_GetSplitToken(BlackBox b){
	return b->split_token;
}

Bool BlackBox_Depends(const BlackBox b1, const BlackBox b2){
/* if b1-->b2 for the quotient graph*/
	int i,n;
	n = IntegerList_Size(&b2->add);
	for(i=1;i<=n;i++){
		if (IntegerList_Contains(&b1->eval,IntegerList_Get(&b2->add,i))) return TRUE;
	}
	n = IntegerList_Size(&b2->del);
	for(i=1;i<=n;i++){
		if (IntegerList_Contains(&b1->eval,IntegerList_Get(&b2->del,i))) return TRUE;
	}
	n = IntegerList_Size(&b2->add);
	for(i=1;i<=n;i++){
		if (IntegerList_Contains(&b1->del,IntegerList_Get(&b2->add,i))) return TRUE;
	}
	return FALSE;
}

Bool BlackBox_IsFormula(BlackBox b){
	return b->formula != NULL;
}
Bool BlackBox_IsEffFormula(BlackBox b){
	return b->eff_formula != NULL;
}
Bool BlackBox_IsSplitToken(BlackBox b){
	return b->split_token != NULL;
}

void BlackBox_Destroy(BlackBox b){
	free(b);
}


void BlackBox_Print(const BlackBox b){
	printf("%s\n", "******** BlackBox_Print ********");
	IntegerList_Print(&b->eval);
	IntegerList_Print(&b->add);
	IntegerList_Print(&b->del);
	IntegerList_Print(&b->free_variables);

}








