#include <math.h>

#include "ff.h"
#include "inst_pre.h"
#include "ServicesAction.h"
#include "ServicesDomain.h"


char* ServicesAction_GetName(Operator_pointer action){
	return action->name;

}
int ServicesAction_GetInterfaceSize(Operator_pointer action){
	return action->num_vars;
}
WffNode *ServicesAction_GetPrecondition(Operator_pointer action){
	return action->preconds;
}
Effect *ServicesAction_GetEffect(Operator_pointer action){
	return action->effects;
}


int ServicesAction_GetAtomArity(Fact *atom){
	int i = 0;
	for(i=0;i<MAX_ARITY;i++){
		int argument  = atom->args[i];
		if (argument==0) break;
	}
	return i;

}

Bool ServicesAction_isVar(Operator *op, int argument){
	if(argument < 0) return TRUE;
	else return FALSE;
}
Bool ServicesAction_isConstant(Operator *op, int argument){
	if(argument > 0) return TRUE;
	else return FALSE;
}

Bool ServicesAction_isFreeVariable(Operator *op, int var){
	return -var <= op->num_vars;
}

float ServicesAction_GetGrounded(Operator *op){
	int i;
	float grounded = 0.0;
	for(i=0; i<op->num_vars; i++){
		int x = ServicesDomain_GetTypeSize(op->var_types[i]);
		float value;
		value = (x == 0) ? 0 : log10(x+10) ;
		grounded+= value;
	}
	return grounded;
}

float ServicesAction_GetTypesAverage(Operator *op){
	int i;
	float avg = 0.0;
	for(i=0; i<op->num_vars; i++){
		int g = ServicesDomain_GetTypeSize(op->var_types[i]);
		avg+= g;
	}
	avg = avg/op->num_vars;
	return avg;
}
