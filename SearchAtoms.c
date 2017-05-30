#include "ff.h"
#include "inst_pre.h"
#include "SearchAtoms.h"

/*HINT: see ADL grammar syntax*/

void SearchAtoms_FromFormula(WffNode* formula, IntegerList *eval){
	if(formula->connective == ATOM){
		/*if the found atom is not in list, then we must to save it*/
		if (!IntegerList_Contains(eval,formula->fact->predicate)){
			IntegerList_Add(eval,formula->fact->predicate);
		}
	}else if(formula->connective == NOT || formula->connective == ALL || formula->connective == EX){
		SearchAtoms_FromFormula(formula->son, eval);
	}else if(formula->connective == AND || formula->connective == OR){
		SearchAtoms_FromFormula(formula->sons, eval);
		WffNode *next = formula->sons->next;
		while (next!=NULL){
			SearchAtoms_FromFormula(next,eval);
			next = next->next;
		}
	}else{ /*case connective == TRU or FAL*/

	}

}




void SearchAtoms_FromAtomicEffs(Literal *effects, IntegerList *add, IntegerList *del ){
	/* <ATOMIC-EFFS>::= <literal> | (and <literal> <literal>+) */
	Literal *current = effects;
	while (current !=NULL){
		if (current->negated){
			if(!IntegerList_Contains(del,current->fact.predicate)){ /*if the found atom is not in del list, then we must to save it*/
				IntegerList_Add(del, current->fact.predicate);
			}
		}else{
			if(!IntegerList_Contains(add,current->fact.predicate)){ /*if the found atom is not in add list, then we must to save it*/
				IntegerList_Add(add, current->fact.predicate);
			}
		}
		current = current->next;
	}

}


void SearchAtoms_FromOneEffFormula(Effect *effect, IntegerList *eval, IntegerList *add, IntegerList *del){
	/* <ONE-EFF-FORMULA>::= <ATOMIC-EFFS>
	 * 						|(when <FORMULA> <ATOMIC-EFFS>)
	 * 						|(forall <VARS> <ATOMIC-EFFS>)
	 * 						|(forall <VARS> (when <FORMULA> <ATOMIC-EFFS>))
	 */
	SearchAtoms_FromFormula(effect->conditions,eval);
	SearchAtoms_FromAtomicEffs(effect->effects, add, del );
}


void SearchAtoms_FromEffFormula(Effect *effect, IntegerList *eval, IntegerList *add, IntegerList *del){
	/*<EFF-FORMULA>::= <ONE-EFF-FORMULA>
	 * 				   |(and <ONE-EFF-FORMULA> <ONE-EFF-FORMULA>+)
	 *
	 */

	SearchAtoms_FromOneEffFormula(effect,eval,add,del);

	/* The following is done (in SearchBlackBox.c) by the function SearchBlackBox_FromEffFormula(...)*/
	Effect * next = effect->next;
	while (next != NULL){
		SearchAtoms_FromOneEffFormula(next,eval,add,del);
		next = next->next;
	}
}















