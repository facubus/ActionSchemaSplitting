
#ifndef __BLACKBOX_H
#define __BLACKBOX_H

#include "IntegerList.h"
#include "SplitToken.h"

typedef struct _BlackBox{
	IntegerList eval;
	IntegerList add;
	IntegerList del;

	IntegerList free_variables;

	WffNode *formula;
	Effect *eff_formula;
	SplitToken split_token;

} _BlackBox, *BlackBox;



BlackBox BlackBox_Empty();

void BlackBox_AddEval(BlackBox b, int atom);
void BlackBox_AddAdd(BlackBox b, int atom);
void BlackBox_AddDel(BlackBox b, int atom);
void BlackBox_AddFreeVariable(BlackBox b, int var);


BlackBox BlackBox_CreateFromFormula(WffNode* formula, int num_vars);
BlackBox BlackBox_CreateFromEffFormula(Effect * eff_formula,int num_vars);
BlackBox BlackBox_CreateFromSplitToken(SplitToken token);


int BlackBox_GetEval(const BlackBox b, int index); /*1 <= index <= size*/
int BlackBox_GetEvalSize(const BlackBox b);

int BlackBox_GetAdd(const BlackBox b, int index); /*1 <= index <= size*/
int BlackBox_GetAddSize(const BlackBox b);

int BlackBox_GetDel(const BlackBox b, int index); /*1 <= index <= size*/
int BlackBox_GetDelSize(const BlackBox b);

int BlackBox_GetAmountFreeVariables(const BlackBox b);
int BlackBox_GetFreeVariable(const BlackBox b, const int index); /* 1 <= index <= GetAmountFreeVariables */


WffNode *BlackBox_GetFormula(BlackBox b);
Effect *BlackBox_GetEffFormula(BlackBox b);
SplitToken BlackBox_GetSplitToken(BlackBox b);


Bool BlackBox_Depends(const BlackBox b1, const BlackBox b2); /*if b1-->b2 for the quotient graph*/

Bool BlackBox_IsFormula(BlackBox b);
Bool BlackBox_IsEffFormula(BlackBox b);
Bool BlackBox_IsSplitToken(BlackBox b);

void BlackBox_Destroy();
void BlackBox_Print(const BlackBox b);

#endif
