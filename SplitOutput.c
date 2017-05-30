#include "ff.h"
#include "inst_pre.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ServicesDomain.h"
#include  "ServicesAction.h"
#include "SplitOutput.h"
#include "InterfaceGraph.h"


void SplitOutput_DomainName(FILE *f){
	fprintf(f, "(domain %s)\n", gdomain_name);
}

void SplitOutput_Requirements(FILE *f){
	fprintf(f, "(:requirements :adl");
	fprintf(f, ")\n");

}

void SplitOutput_Types(FILE *f){
	if (gparse_types != NULL){ /*there are types*/
		fprintf(f, "(:types\n");
		TypedList *current = gparse_types;
		while (current != NULL){
			fprintf(f, "\t%s - %s\n",  current->name, current->type->item);
			current = current->next;
		}
		fprintf(f, ")\n");
	}

}

void SplitOutput_Constants(FILE *f){
	if (gparse_constants != NULL){ /*there are constants*/
		fprintf(f, "(:constants\n");
		TypedList *current = gparse_constants;
		while (current != NULL){
			fprintf(f, "\t%s - %s\n",  current->name, current->type->item);
			current = current->next;
		}
		fprintf(f, ")\n");
	}
}




void SplitOutput_Predicates(FILE *f, SplitDomain splitDomain){

	/*print originals predicates*/
	fprintf(f, "(:predicates\n");
	TypedListList *current_predicate = gparse_predicates;
	while (current_predicate != NULL){
		fprintf(f, "\t(%s", current_predicate->predicate);
		TypedList * current_arg = current_predicate->args;
		while (current_arg){
			fprintf(f, " %s - %s", current_arg->name, current_arg->type->item);
			current_arg = current_arg->next;
		}
		fprintf(f, ")\n");
		current_predicate = current_predicate->next;
	}


	/*print token procnone*/
	fprintf(f, "\t(procnone)\n");

	/*print token do*/
	int i,n = SplitDomain_GetSize(splitDomain);
	for (i = 1; i <= n; i++){
		int j,m = SplitAction_AmountSubActions(SplitDomain_GetSplitAction(splitDomain,i));
		for (j = 2; j <= m; j++){
			fprintf(f, "\t(do_%d_%d)\n", i,j);
		}
	}

	/*print token arg*/
	n = ServicesDomain_GetMaxInterface();
	for (i = 1; i <= n; i++){
		fprintf(f, "\t(arg_%d ?x - object)\n", i);
	}

	fprintf(f, ")\n");
}




void SplitOutput_WffNode(FILE *f, WffNode *formula, Operator *op){

	switch (formula->connective){
	case TRU:
		  fprintf(f, "true");
		  break;
	case FAL:
		  fprintf(f, "false");
		  break;
	case ATOM:
		SplitOutput_Atom(f,formula->fact,op);
		break;
	case NOT:
		SplitOutput_Not(f,formula,op);
		break;
	case AND:
		SplitOutput_And(f,formula,op);
		break;
	case OR:
		SplitOutput_Or(f,formula,op);
		break;
	case ALL:
		SplitOutput_All(f,formula,op);
		break;
	case EX:
		SplitOutput_Ex(f,formula,op);
		break;
	default:
	    printf("\nWff output: %d is wrong Node specifier\n", formula->connective);
	}

}


void SplitOutput_Atom(FILE *f, Fact *fact, Operator *op){
	  fprintf(f, "(%s", gpredicates[fact->predicate]);
	  int j;
	  for ( j=0; j<garity[fact->predicate]; j++ ) {
		  if (fact->args[j] >= 0)
			  fprintf(f, " %s", gconstants[fact->args[j]]);
		  else{
			  int var = -fact->args[j];
			  fprintf(f, " %s", op->var_names[var-1]);
		  }
	  }
	  fprintf(f, ")");
}

void SplitOutput_Not(FILE *f, WffNode *formula, Operator *op){
	  fprintf(f, "(not ");
	  SplitOutput_WffNode(f, formula->son,op);
	  fprintf(f, ")");
}

void SplitOutput_And(FILE *f, WffNode *formula, Operator *op){
	  fprintf(f, "(and ");
	  WffNode *current  = formula->sons;
	  while (current != NULL){
		  SplitOutput_WffNode(f, current,op);
		  current= current ->next;
	  }
	  fprintf(f, ")");
}

void SplitOutput_Or(FILE *f, WffNode *formula, Operator *op){
	  fprintf(f, "(or ");
	  WffNode *current  = formula->sons;
	  while (current != NULL){
		  SplitOutput_WffNode(f, current, op);
		  current= current ->next;
	  }
	  fprintf(f, ")");
}

void SplitOutput_All(FILE *f, WffNode *formula, Operator *op){
	  fprintf(f, "(forall (?x%d - %s) ", formula->var+1, gtype_names[formula->var_type]);
	  SplitOutput_WffNode(f, formula->son, op);
	  fprintf(f, ")");
}

void SplitOutput_Ex(FILE *f, WffNode *formula, Operator *op){
	  fprintf(f, "(exists (?x%d - %s) ", formula->var+1, gtype_names[formula->var_type]);
	  SplitOutput_WffNode(f, formula->son, op);
	  fprintf(f, ")");
}


void SplitOutput_Effect(FILE *f, Effect *eff_formula, Operator *op){
	Bool b = eff_formula->next != NULL; /*more than one literal*/
	if (b) fprintf(f, "(and ");

	Effect *current = eff_formula;
	while(current != NULL){
		SplitOutput_OneEffFormula(f,current, op);
		current = current->next;
	}
	if (b) fprintf(f, ")"); /*more than one literal*/
}


void SplitOutput_AtomicEffs(FILE *f, Effect *eff_formula, Operator *op){
	Literal *effects = eff_formula->effects;
	Bool b = effects->next != NULL; /*more than one literal*/
	if (b) fprintf(f, "(and ");

	while (effects != NULL){
		if(effects->negated){
			fprintf(f, "(not");
			SplitOutput_Atom(f, &effects->fact,op);
			fprintf(f, ")");
		}else{
			SplitOutput_Atom(f, &effects->fact,op);
		}
		effects = effects->next;

	}
	if (b) fprintf(f, ")"); /*more than one literal*/
}

void SplitOutput_OneEffFormula(FILE *f, Effect *eff_formula, Operator *op){

	if (eff_formula->num_vars == 0 && eff_formula->conditions->connective ==TRU){ /*case <atomic-effs>*/
		SplitOutput_AtomicEffs(f,eff_formula,op);

	}else if(eff_formula->num_vars == 0 && eff_formula->conditions->connective !=TRU){ /*case when <formula> <atomic-effs>*/
		fprintf(f, "(when ");
		SplitOutput_WffNode(f,eff_formula->conditions,op);
		SplitOutput_AtomicEffs(f,eff_formula,op);
		fprintf(f, ")");

	}else if(eff_formula->num_vars != 0 && eff_formula->conditions->connective ==TRU){ /*case forall <atomic-effs>*/
		fprintf(f, "(forall (");
		int i;
		for(i=0; i<eff_formula->num_vars; i++){
			fprintf(f, "?x%d - %s ", op->num_vars+i+1, gtype_names[eff_formula->var_types[i]]);
		}
		fprintf(f, ")");
		SplitOutput_AtomicEffs(f,eff_formula,op);
		fprintf(f, ")");
	}else{ /*case forall when <formula> <atomic-effs>*/
		fprintf(f, "(forall (");
		int i;
		for(i=0; i<eff_formula->num_vars; i++){
			fprintf(f, "?x%d - %s ", op->num_vars+i+1, gtype_names[eff_formula->var_types[i]]);
		}
		fprintf(f, ")");
		fprintf(f, "(when ");
		SplitOutput_WffNode(f,eff_formula->conditions,op);
		SplitOutput_AtomicEffs(f,eff_formula,op);
		fprintf(f, ")");
		fprintf(f, ")");

	}
}



void SplitOutput_SplitToken(FILE *f, SplitToken token, Operator *op){
	if (SplitToken_isProcnone(token)){
		if (token->positive){
			fprintf(f, "(procnone)");
		}else{
			fprintf(f, "(not(procnone))");
		}
	}
	if (SplitToken_isDo(token)){
		if (token->positive){
			fprintf(f, "(do_%d_%d)", token->action, token->subaction);
		}else{
			fprintf(f, "(not(do_%d_%d))", token->action, token->subaction);
		}
	}
	if (SplitToken_isArg(token)){
		int var = -token->var;
		if (token->positive){
			fprintf(f, "(arg_%d %s)", var, op->var_names[var-1]);
		}else{
			fprintf(f, "(not(arg_%d %s))", var, op->var_names[var-1]);
		}
	}

}


void SplitOutput_SubPartition(FILE *f, SubPartition sp, Operator *op, int order){
	/* subaction name*/
	fprintf(f, "(:action %s_%d\n", op->name, order);

	/* subaction parameters */
	fprintf(f, ":parameters(");
	int j,t = SubPartition_GetInterfaceSize(sp);
	for(j = 1;j <= t; j++){
		int var = -SubPartition_GetVar(sp,j);
		fprintf(f,"%s - %s ", op->var_names[var-1], gtype_names[op->var_types[var - 1]]);
	}
	fprintf(f, ")\n");


	/*subaction precondition*/
	fprintf(f, ":precondition ");
	int n = SubPartition_GetPreSize(sp);
	int m = SubPartition_GetSplitTokenPreSize(sp);
	if (n + m >= 2) fprintf(f, "(and ");

	int i;
	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetPreBlackBox(sp,i);
		SplitOutput_WffNode(f, BlackBox_GetFormula(b),op);
	}

	for(i = 1; i <= m; i++){
		SplitOutput_SplitToken(f,SubPartition_GetSplitTokenPre(sp,i),op);
	}

	if (n + m >= 2) fprintf(f, ")");

	fprintf(f, "\n");

	/*subaction effect*/
	fprintf(f, ":effect ");
	n = SubPartition_GetEffSize(sp);
	m = SubPartition_GetSplitTokenEffSize(sp);
	if (n + m >= 2) fprintf(f, "(and ");

	for(i = 1; i <= n; i++){
		BlackBox b = SubPartition_GetEffBlackBox(sp,i);
		SplitOutput_Effect(f, BlackBox_GetEffFormula(b),op);
	}

	for(i = 1; i <= m; i++){
		SplitOutput_SplitToken(f,SubPartition_GetSplitTokenEff(sp,i),op);
	}

	if (n + m >= 2) fprintf(f, ")");
	if (n + m == 0) fprintf(f, "()");
	fprintf(f, "\n)");

}



void SplitOutput_SplitAction(FILE *f, SplitAction splitAction){
	int i, n = SplitAction_AmountSubActions(splitAction);
	for(i = 1; i <= n; i++){
		fprintf(f, "\n");
		SplitOutput_SubPartition(f, SplitAction_GetSubAction(splitAction,i), SplitAction_GetOperator(splitAction), i);
		fprintf(f, "\n");
	}
}

void SplitOutput_SplitDomain(char *file, SplitDomain splitDomain){
	FILE *f = fopen(file, "w");
	fprintf(f, "(define ");
	SplitOutput_DomainName(f);
	SplitOutput_Requirements(f);
	SplitOutput_Types(f);
	SplitOutput_Constants(f);
	SplitOutput_Predicates(f, splitDomain);

	/* subactions */
	int i, n = SplitDomain_GetSize(splitDomain);
	for(i = 1; i <= n; i++){
		/*printf("%s\n", SplitAction_GetOperator(SplitDomain_GetSplitAction(splitDomain,i))->name);*/
		SplitOutput_SplitAction(f,SplitDomain_GetSplitAction(splitDomain,i));
	}
	fprintf(f, "\n)");
	fclose(f);
}



void SplitOutput_ProblemName(FILE *f){
	fprintf(f, "(problem %s) (:domain %s)\n", gproblem_name,  gdomain_name);
}

void SplitOutput_Objects(FILE *f){
	fprintf(f, "(:objects\n");
	TypedList *current = gparse_objects;
	while (current != NULL){
	  fprintf(f, "\t%s - %s\n", current->name, current->type->item);
	  current = current->next;
	}

	fprintf(f, ")\n");
}


void SplitOutput_Init(FILE *f){
	fprintf(f, "(:init\n");
	int i;
    for ( i = 0; i < gnum_full_initial; i++ ) {
    	if (!gfull_initial[i].predicate == 0){
    		fprintf(f, "\t");
    		SplitOutput_Atom(f, &(gfull_initial[i]), NULL); /*NULL becaause the initial state is a instantiated conjunction formula*/
			fprintf(f, "\n");
    	}
    }
	fprintf(f, "\t(procnone)\n");
	fprintf(f, ")\n");
}


void SplitOutput_Goal(FILE *f){
	fprintf(f, "(:goal ");
	if (ggoal->connective != AND){
		fprintf(f, "(and (procnone) ");
		SplitOutput_WffNode(f,ggoal, NULL); /*NULL becaause the goal is an instantiated formula*/
		fprintf(f, "))\n");

	}else{
		fprintf(f, "(and (procnone) ");
		WffNode *current = ggoal->sons;
		while (current != NULL){
			SplitOutput_WffNode(f,current, NULL); /*NULL becaause the goal is an instantiated formula*/
			current = current->next;
		}
		fprintf(f, "))\n");


	}
}


void SplitOutput_SplitProblem(char *file){
	FILE *f = fopen(file, "w");
	fprintf(f, "(define ");
	SplitOutput_ProblemName(f);
	SplitOutput_Objects(f);
	SplitOutput_Init(f);
	SplitOutput_Goal(f);
	fprintf(f, "\n)");
	fclose(f);
}



void SplitOutput_SplitLog(char *file, SplitDomain splitDomain){
	FILE *f = fopen(file, "w");
	int i, n = SplitDomain_GetSize(splitDomain);
	for(i = 1; i <= n; i++){
		SplitAction a = SplitDomain_GetSplitAction(splitDomain,i);
		int j, m = SplitAction_AmountSubActions(a);
		for(j = 1; j <= m; j++){
			fprintf(f, "%s %d - ", SplitAction_GetName(a),j);
			SubPartition sp = SplitAction_GetSubAction(a,j);
			int w = SubPartition_GetInterfaceSize(sp);
			int k;
			for(k = 1; k <= w; k++){
				fprintf(f, "%d ", -1*SubPartition_GetVar(sp,k));
			}
			fprintf(f, "\n");
		}
	}

	fclose(f);


}



void SplitOutput_SplittedOperators(char *splitted_operator_path, SplitDomain splitDomain){
	FILE *f = fopen(splitted_operator_path, "w");
	int i;
	for(i=1; i<= SplitDomain_GetSplittedOperatorSize(splitDomain); i++){
		int operator_index = SplitDomain_GetSplittedOperator(splitDomain,i);
		if (operator_index >= 1)
			fprintf(f, "%s\n", ServicesAction_GetName(ServicesDomain_GetAction(operator_index)));
		else
			fprintf(f, "null");
	}
	fclose(f);


}




void SplitOutput_SplitabilityGraph(char *splitability_graph_path){
	/*write*/
	FILE *f = fopen(splitability_graph_path,"w");
	fprintf(f, "graph \"%s\" {\n", ServicesDomain_GetName());
	int i;
	for (i=1; i<=ServicesDomain_GetAmountActions();i++){
		Operator *op = ServicesDomain_GetAction(i);
		InterfaceGraph g = InterfaceGraph_Create(op);
		InterfaceGraph_ToFile(f,g);
	}
	fprintf(f, "}");
	fclose(f);

}













































