#include "ff.h"
#include "inst_pre.h"

#include "SplitToken.h"



SplitToken SplitToken_Create(int type, int action, int subaction, int var, Bool positive){
	SplitToken token = (SplitToken)malloc(sizeof(_SplitToken));
	token->type = type;
	token->action = action;
	token->subaction = subaction;
	token->var = var;
	token->positive = positive;
	return token;
}


Bool SplitToken_isProcnone(SplitToken token){
	return token->type == 1;
}
Bool SplitToken_isDo(SplitToken token){
	return token->type == 2;
}
Bool SplitToken_isArg(SplitToken token){
	return token->type == 3;
}


void SplitToken_Destroy(SplitToken token){
	free(token);
}

void SplitToken_Print(SplitToken token){

	switch (token->type){
		case 1:
			printf("(proc_none)");
			break;
		case 2:
			printf("(do_%d_%d)", token->action, token->subaction);
			break;
		case 3:
			printf("(arg_%d ?x%d)", -token->var, -token->var);
			break;
		default:
		    printf("\nSplitToken print function: %d is wrong Type specifier\n", token->type);
	}

}










