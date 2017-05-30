
#ifndef __SEARCHATOMS_H
#define __SEARCHATOMS_H

#include "IntegerList.h"




void SearchAtoms_FromFormula(WffNode* formula, IntegerList *eval);


void SearchAtoms_FromAtomicEffs(Literal *effects, IntegerList *add, IntegerList *del );
void SearchAtoms_FromOneEffFormula(Effect *effect, IntegerList *eval, IntegerList *add, IntegerList *del);
void SearchAtoms_FromEffFormula(Effect *effect, IntegerList *eval, IntegerList *add, IntegerList *del);


#endif
