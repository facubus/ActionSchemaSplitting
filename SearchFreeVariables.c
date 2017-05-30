#include "ff.h"
#include "inst_pre.h"
#include "SearchFreeVariables.h"

/*HINT: see ADL grammar syntax*/

void SearchFreeVariables_FromFormula(const WffNode* formula, int num_vars, IntegerList *free_variables){
	if(formula->connective == ATOM){
		/*if the variable found is an operator's variable, then it is a free variable, then we must to save it*/
		int i;
		for (i=0;i<MAX_ARITY;i++){
			int var = formula->fact->args[i];
			if (var < 0 && -1*var <= num_vars && !IntegerList_Contains(free_variables, var)){
				IntegerList_Add(free_variables,var);
			}
		}
	}else if(formula->connective == NOT || formula->connective == ALL || formula->connective == EX){
		SearchFreeVariables_FromFormula(formula->son, num_vars,free_variables);

	}else if(formula->connective == AND || formula->connective == OR){
		SearchFreeVariables_FromFormula(formula->sons, num_vars, free_variables);
		WffNode *next = formula->sons->next;
		while (next!=NULL){
			SearchFreeVariables_FromFormula(next, num_vars, free_variables);
			next = next->next;
		}
	}else{ /* case connective == {TRU,FAL} */

	}

}





void SearchFreeVariables_FromAtomicEffs(Literal *effects, int num_vars, IntegerList *free_variables){
	/* <ATOMIC-EFFS>::= <literal> | (and <literal> <literal>+) */
	Literal *current = effects;
	while (current !=NULL){
		int i;
		for (i = 0; i < MAX_ARITY; i++){
			int var = current->fact.args[i];
			if (var < 0 && -1*var <= num_vars && !IntegerList_Contains(free_variables, var)){
				IntegerList_Add(free_variables,var);
			}

		}
		current = current->next;
	}
}


void SearchFreeVariables_FromOneEffFormula(const Effect *effect, int num_vars, IntegerList *free_variables){
	/* <ONE-EFF-FORMULA>::= <ATOMIC-EFFS>
	 * 						|(when <FORMULA> <ATOMIC-EFFS>)
	 * 						|(forall <VARS> <ATOMIC-EFFS>)
	 * 						|(forall <VARS> (when <FORMULA> <ATOMIC-EFFS>))
	 */
	SearchFreeVariables_FromFormula(effect->conditions,num_vars, free_variables);
	SearchFreeVariables_FromAtomicEffs(effect->effects, num_vars, free_variables);
}


void SearchFreeVariables_FromEffFormula(const Effect *effect, int num_vars, IntegerList *free_variables){
	/*<EFF-FORMULA>::= <ONE-EFF-FORMULA>
	 * 				   |(and <ONE-EFF-FORMULA> <ONE-EFF-FORMULA>+)
	 *
	 */
	SearchFreeVariables_FromOneEffFormula(effect, num_vars, free_variables);
	Effect *next = effect->next;
	while (next!=NULL){
		SearchFreeVariables_FromOneEffFormula(next, num_vars, free_variables);
		next = next->next;
	}

}















