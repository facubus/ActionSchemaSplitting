
#ifndef __SERVICESACTION_H
#define __SERVICESACTION_H


char* ServicesAction_GetName(Operator_pointer action);
int ServicesAction_GetInterfaceSize(Operator_pointer action);
WffNode* ServicesAction_GetPrecondition(Operator_pointer action);
Effect* ServicesAction_GetEffect(Operator_pointer action);


int ServicesAction_GetAtomArity(Fact *atom);
Bool ServicesAction_isVar(Operator *op, int argument);
Bool ServicesAction_isConstant(Operator *op, int argument);
Bool ServicesAction_isFreeVariable(Operator *op, int var);

float ServicesAction_GetGrounded(Operator *op);
float ServicesAction_GetTypesAverage(Operator *op);

#endif
