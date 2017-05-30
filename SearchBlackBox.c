#include "ff.h"
#include "inst_pre.h"
#include "SearchBlackBox.h"

void SearchBlackBox_FromFormula(WffNode* formula, BlackBoxList *l, int num_vars){
	if(formula->connective == AND){
		SearchBlackBox_FromFormula(formula->sons, l, num_vars);
		WffNode *next = formula->sons->next;
		while (next!=NULL){
			SearchBlackBox_FromFormula(next,l, num_vars);
			next = next->next;
		}

	}else{
		BlackBox b = BlackBox_CreateFromFormula(formula, num_vars);
		BlackBoxList_Add(l,b);
	}
}


void SearchBlackBox_FromOneEffFormula(Effect *effect, BlackBoxList *l, int num_vars){
	if (effect->num_vars != 0){ /* effect is a FORALL */
		/*printf("%s\n", "all");*/
		BlackBox b = BlackBox_CreateFromEffFormula(effect, num_vars);
		BlackBoxList_Add(l,b);
	}else{ /* effect is WHEN or a <ATOMIC-EFFS> */
		if(effect->conditions->connective != TRU){ /* effect is a WHEN */
			/*printf("%s\n", "when");*/
			BlackBox b = BlackBox_CreateFromEffFormula(effect, num_vars);
			BlackBoxList_Add(l,b);
		}else{ /* effect is a <ATOMIC-EFFS> */
			/*printf("%s\n", "lit");*/
			Literal *current  = effect->effects;
			while (current!=NULL){
				BlackBox b = BlackBox_CreateFromLiteral(current, num_vars);
				BlackBoxList_Add(l,b);
				current = current->next;
			}
		}
	}

}

void SearchBlackBox_FromEffFormula(Effect *effect, BlackBoxList *l, int num_vars){
	SearchBlackBox_FromOneEffFormula(effect, l, num_vars);

	Effect * next = effect->next;
	while (next != NULL){
		SearchBlackBox_FromOneEffFormula(next, l, num_vars);
		next = next->next;
	}
}



