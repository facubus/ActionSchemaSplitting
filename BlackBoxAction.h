
#ifndef __BLACKBOXACTION_H
#define __BLACKBOXACTION_H

#include "BlackBoxList.h"

typedef struct _BlackBoxAction{
	Operator *op;
	BlackBoxList precondition;
	BlackBoxList effect;

	int N1; /*size(atom split)*/
	int N2; /*interface_size(trivial  split)*/

} _BlackBoxAction, *BlackBoxAction;



BlackBoxAction BlackBoxAction_Create(Operator* op);


char *BlackBoxAction_GetName(const BlackBoxAction a);
BlackBox BlackBoxAction_GetPrecondition(const BlackBoxAction a, int index);
BlackBox BlackBoxAction_GetEffect(const BlackBoxAction a, int index);

int BlackBoxAction_GetPreconditionSize(const BlackBoxAction a);
int BlackBoxAction_GetEffectSize(const BlackBoxAction a);

void BlackBoxAction_Print(const BlackBoxAction b);
void BlackBoxAction_Destroy(const BlackBoxAction a);




#endif
