
#ifndef __SEARCHFREEVARIABLES_H
#define __SEARCHFREEVARIABLES_H


#include "IntegerList.h"

void SearchFreeVariables_FromFormula(const WffNode* formula, int num_vars, IntegerList *free_variables);



void SearchFreeVariables_FromAtomicEffs(Literal *effects, int num_vars, IntegerList *free_variables);
void SearchFreeVariables_FromOneEffFormula(const Effect *effect, int num_vars, IntegerList *free_variables);
void SearchFreeVariables_FromEffFormula(const Effect *effect, int num_vars, IntegerList *free_variables);




#endif
