#include "InterfaceGraph.h"
#include "ServicesDomain.h"
#include "ServicesAction.h"
#include "dgraph.h"
#include "sc.h"


InterfaceGraph InterfaceGraph_Create(Operator *op){

	InterfaceGraph g = (InterfaceGraph)malloc(sizeof(_InterfaceGraph));
	g->op = op;
	g->num_nodes = op->num_vars;

	/* matrix's initialization */
	g->matrix = (int**) malloc(g->num_nodes*sizeof(int *));
	int i;
	for (i=0;i<g->num_nodes;i++){
		g->matrix[i] = (int *) calloc (g->num_nodes,sizeof(int));
	}

	InterfaceGraph_FromFormula(g,op->preconds);
	InterfaceGraph_FromEffFormula(g,op->effects);
	InterfaceGraph_FromTyped(g);

	return g;
}





void InterfaceGraph_PrintVar(InterfaceGraph g, int var){
	printf("v%d	", var);
	int j;
	for (j=0;j<g->num_nodes;j++){
		printf("%d ", g->matrix[var-1][j]);
	}
	printf("\n");

}

void InterfaceGraph_Print(InterfaceGraph g){
	printf("%s\n", g->op->name);
	int v;
	for (v=1; v<=g->num_nodes; v++){
		InterfaceGraph_PrintVar(g, v);
	}
}


int InterfaceGraph_GetNumNodes(InterfaceGraph g){
	return g->num_nodes;
}

void InterfaceGraph_AddEdge(InterfaceGraph g, int var1, int var2, int arity){
	if (g->matrix[-var1-1][-var2-1] < arity)
		g->matrix[-var1-1][-var2-1] = arity;
		g->matrix[-var2-1][-var1-1] = arity; /*matrix is symetric, therefore update both values */
}




void InterfaceGraph_FromFormula(InterfaceGraph g, WffNode *formula){
	if(formula->connective == ATOM){
		int arity = ServicesAction_GetAtomArity(formula->fact);
		int i,j;
		for(i=0;i<arity;i++){
			for(j=i+1;j<arity;j++){
				int x = formula->fact->args[i];
				int y = formula->fact->args[j];
				if(ServicesAction_isVar(g->op, x) && ServicesAction_isVar(g->op, y)
						&& ServicesAction_isFreeVariable(g->op, x) && ServicesAction_isFreeVariable(g->op, y)){
					InterfaceGraph_AddEdge(g,x,y,arity);
				}
			}
		}
	}else if(formula->connective == NOT || formula->connective == ALL || formula->connective == EX){
		InterfaceGraph_FromFormula(g, formula->son);

	}else if(formula->connective == AND || formula->connective == OR){
		InterfaceGraph_FromFormula(g,formula->sons);
		WffNode *next = formula->sons->next;
		while (next!=NULL){
			InterfaceGraph_FromFormula(g,next);
			next = next->next;
		}

	}else{ /*case connective == TRU or FAL*/

	}
}



void InterfaceGraph_FromAtomicEffs(InterfaceGraph g, Literal *effects){
	/* <ATOMIC-EFFS>::= <literal> | (and <literal> <literal>+) */
	Literal *current = effects;
	while (current !=NULL){
		int arity = ServicesAction_GetAtomArity(&current->fact);
		int i,j;
		for(i=0;i<arity;i++){
			for(j=i+1;j<arity;j++){
				int x = current->fact.args[i];
				int y = current->fact.args[j];
				if(ServicesAction_isVar(g->op, x) && ServicesAction_isVar(g->op, y)
						&& ServicesAction_isFreeVariable(g->op, x) && ServicesAction_isFreeVariable(g->op, y)){
					InterfaceGraph_AddEdge(g,x,y,arity);
				}

			}
		}
		current = current->next;
	}
}


void InterfaceGraph_FromOneEffFormula(InterfaceGraph g, Effect *effect){
	/* <ONE-EFF-FORMULA>::= <ATOMIC-EFFS>
	 * 						|(when <FORMULA> <ATOMIC-EFFS>)
	 * 						|(forall <VARS> <ATOMIC-EFFS>)
	 * 						|(forall <VARS> (when <FORMULA> <ATOMIC-EFFS>))
	 */
	InterfaceGraph_FromFormula(g,effect->conditions);
	InterfaceGraph_FromAtomicEffs(g, effect->effects);
}


void InterfaceGraph_FromEffFormula(InterfaceGraph g, Effect *effect){
	/*<EFF-FORMULA>::= <ONE-EFF-FORMULA>
	 * 				   |(and <ONE-EFF-FORMULA> <ONE-EFF-FORMULA>+)
	 *
	 */

	InterfaceGraph_FromOneEffFormula(g, effect);
	Effect * next = effect->next;
	while (next != NULL){
		InterfaceGraph_FromEffFormula(g,next);
		next = next->next;
	}
}





void InterfaceGraph_FromTyped(InterfaceGraph g){
	int i;
	for(i=0; i<g->op->num_vars; i++){
		 g->matrix[i][i] = ServicesDomain_GetTypeSize(g->op->var_types[i]); /*put the variable's grounded in the matrix' diagonal*/
	}
}


float InterfaceGraph_Density(InterfaceGraph g){
	if (g->num_nodes < 2) return 1.0;
	int i,j;
	int sum = 0.0;
	for(i=0;i<g->num_nodes;i++){
		for(j=i+1;j<g->num_nodes;j++){
			sum+= g->matrix[i][j];
		}
	}
	int denom = (g->num_nodes * g->num_nodes) * (g->num_nodes - 1) / 2;
	return (float)sum/denom;
}


int InterfaceGraph_Size(InterfaceGraph g){
	return g->num_nodes;
}


float InterfaceGraph_Grounded(InterfaceGraph g){
	return ServicesAction_GetGrounded(g->op);
}



int InterfaceGraph_StronglyConnectedComponents(InterfaceGraph g){
	dgraph_t *gg = dgraph_blank(g->num_nodes);
	int i,j;
	for (i = 0; i < g->num_nodes; i++){
		for (j = i+1; j < g->num_nodes; j++){
			if (g->matrix[i][j] != 0 ){
				dgraph_add_new_edge(gg,i,j);
				dgraph_add_new_edge(gg,j,i);
			}
		}
	}
	sc_result_t *scc_result = sc(gg);
	return scc_result->n_sets;
}




void InterfaceGraph_ToFile(FILE *f, InterfaceGraph g){

	int operator_number = ServicesDomain_GetActionNumber(g->op);
	fprintf(f, "subgraph cluster%d{\n", operator_number);

	int i,j;
	for(i=0;i<g->num_nodes;i++){ /*write variables, their labels, their grounds*/
		fprintf(f, "a%d_%d [label = \"%s:%d\"]\n", operator_number, i+1, g->op->var_names[i] + 1, g->matrix[i][i]);
	}

	for(i=0;i<g->num_nodes;i++){ /*write edges*/
		for(j=i+1; j< g->num_nodes;j++){
			if (g->matrix[i][j] != 0){ /*there is a edge between the variables*/
				fprintf(f, "a%d_%d -- a%d_%d [label = %d]\n", operator_number,i+1, operator_number, j+1, g->matrix[i][j]);

			}
		}
	}

	fprintf(f, "label=\"%s\"\n", g->op->name);
	fprintf(f, "color=blue\n");
	fprintf(f, "}\n");

}







void InterfaceGraph_Main(Operator *op, char *file){
	InterfaceGraph g = InterfaceGraph_Create(op);
	InterfaceGraph_Print(g);
	printf("%d\n", InterfaceGraph_Density(g));
	printf("%d\n", InterfaceGraph_Size(g));
	printf("%d\n", InterfaceGraph_StronglyConnectedComponents(g));
	printf("%f\n", InterfaceGraph_Grounded(g));
	printf("--------------");

	FILE *f = fopen(file,"w");
	InterfaceGraph_ToFile(f,g);
	fclose(f);

}



