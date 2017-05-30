
#ifndef __SEARCHCONJUNCTION_H
#define __SEARCHCONJUNCTION_H

#include "WffNodeList.h"
#include "EffectList.h"

void SearchConjunction_FromFormulaRecursive(WffNode* formula, WffNodeList *l);
WffNodeList SearchConjunction_FromFormula(WffNode* formula);


void SearchConjunction_FromOneEffFormula(Effect *effect, EffectList *l);
EffectList SearchConjunction_FromEffFormula(Effect *effect);



#endif
