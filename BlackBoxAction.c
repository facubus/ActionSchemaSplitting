#include "ff.h"
#include "inst_pre.h"

#include "BlackBoxAction.h"
#include "ServicesAction.h"
#include "SearchBlackBox.h"
#include "SearchConjunction.h"

BlackBoxAction BlackBoxAction_Create(Operator* op){

	BlackBoxAction a = (BlackBoxAction)malloc (sizeof(_BlackBoxAction));
	a->op = op;

	a->precondition = BlackBoxList_Create();
	WffNodeList preconditions = SearchConjunction_FromFormula(op->preconds);
	int i,n = WffNodeList_Size(&preconditions);
	for(i = 1; i <= n; i++){
		BlackBox b = BlackBox_CreateFromFormula(WffNodeList_Get(&preconditions,i),op->num_vars);
		BlackBoxList_Add(&a->precondition, b);
	}

	a->effect = BlackBoxList_Create();
	EffectList effects  = SearchConjunction_FromEffFormula(op->effects);
	n = EffectList_Size(&effects);
	for(i = 1; i <= n; i++){
		BlackBox b = BlackBox_CreateFromEffFormula(EffectList_Get(&effects,i),op->num_vars);
		BlackBoxList_Add(&a->effect,b);
	}

	a->N1 = BlackBoxAction_GetPreconditionSize(a) + BlackBoxAction_GetEffectSize(a);
	a->N2 = ServicesAction_GetInterfaceSize(op);

	return a;
}


char *BlackBoxAction_GetName(const BlackBoxAction a){
	return a->op->name;
}
BlackBox BlackBoxAction_GetPrecondition(const BlackBoxAction a, int index){
	return BlackBoxList_Get(&a->precondition, index);
}
BlackBox BlackBoxAction_GetEffect(const BlackBoxAction a, int index){
	return BlackBoxList_Get(&a->effect, index);
}

int BlackBoxAction_GetPreconditionSize(const BlackBoxAction a){
	return BlackBoxList_Size(&a->precondition);
}

int BlackBoxAction_GetEffectSize(const BlackBoxAction a){
	return BlackBoxList_Size(&a->effect);
}

void BlackBoxAction_Print(const BlackBoxAction a){
	printf("%s %s %s\n", "******** BlackBoxAction ", a->op->name ," ********");
	printf("%s\n", "Precondition:");
	BlackBoxList_Print(&a->precondition);
	printf("%s\n", "Effect:");
	BlackBoxList_Print(&a->effect);
	/*
	 */
}

void BlackBoxAction_Destroy(BlackBoxAction a){
	free(a);
}








