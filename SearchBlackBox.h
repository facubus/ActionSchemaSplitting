
#ifndef __SEARCHBLACKBOX_H
#define __SEARCHBLACKBOX_H

#include "BlackBoxList.h"


void SearchBlackBox_FromFormula(WffNode* formula, BlackBoxList *l, int num_vars);
void SearchBlackBox_FromEffFormula(Effect *effect, BlackBoxList *l, int num_vars);



#endif
