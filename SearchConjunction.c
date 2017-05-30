#include "ff.h"
#include "inst_pre.h"
#include "SearchConjunction.h"

void SearchConjunction_FromFormulaRecursive(WffNode* formula, WffNodeList *l){
	if(formula->connective == AND){
		WffNode *current = formula->sons;
		while (current != NULL){
			SearchConjunction_FromFormulaRecursive(current,l);
			/*WffNodeList_Add(l,current);*/
			current = current->next;
		}
	}else{
		WffNodeList_Add(l,formula);
	}

}

WffNodeList SearchConjunction_FromFormula(WffNode* formula){
	WffNodeList l = WffNodeList_Create();
	SearchConjunction_FromFormulaRecursive(formula,&l);

	int i, n = WffNodeList_Size(&l);
	for(i=1;i<=n;i++){
		WffNodeList_Get(&l,i)->next = NULL;
		WffNodeList_Get(&l,i)->prev = NULL;
	}
	return l;
}



void SearchConjunction_FromOneEffFormula(Effect *effect, EffectList *l){
	if (effect->num_vars == 0 && effect->conditions->connective == TRU){ /*<ATOMIC-EFFS> */
		Literal *current  = effect->effects;
		while (current != NULL){
			Effect *e = (Effect*) malloc(sizeof(Effect));
			e->num_vars = effect->num_vars;
			e->conditions = effect->conditions;
			e->effects = current;

			EffectList_Add(l,e);
			current = current->next;

			e->effects->next = NULL;
			e->effects->prev = NULL;

		}

	}else{ /*FORALL or WHEN*/
		EffectList_Add(l,effect);
	}

}



EffectList SearchConjunction_FromEffFormula(Effect *effect){
	EffectList l = EffectList_Create();
	Effect *current = effect;
	while (current != NULL){
		SearchConjunction_FromOneEffFormula(current, &l);
		current = current->next;
	}

	int i, n = EffectList_Size(&l);
	for(i = 1; i <= n; i++){
		EffectList_Get(&l,i)->next = NULL;
		EffectList_Get(&l,i)->prev = NULL;
	}
	return l;

}





