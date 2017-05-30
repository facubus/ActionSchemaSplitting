
#ifndef __INTERFACEGRAPH_H
#define __INTERFACEGRAPH_H

#include "ff.h"
#include "inst_pre.h"
#include "IntegerList.h"


typedef struct _InterfaceGraph{
	Operator *op;
	int num_nodes;
	int **matrix;

} _InterfaceGraph, *InterfaceGraph;



InterfaceGraph InterfaceGraph_Create(Operator *op);

void InterfaceGraph_PrintVar(InterfaceGraph g, int var);
void InterfaceGraph_Print(InterfaceGraph g);
int InterfaceGraph_GetNumNodes(InterfaceGraph g);
void InterfaceGraph_AddEdge(InterfaceGraph g, int var1, int var2, int arity);

void InterfaceGraph_FromFormula(InterfaceGraph g, WffNode *formula);
void InterfaceGraph_FromAtomicEffs(InterfaceGraph g, Literal *effects);
void InterfaceGraph_FromOneEffFormula(InterfaceGraph g, Effect *effect);
void InterfaceGraph_FromEffFormula(InterfaceGraph g, Effect *effect);
void InterfaceGraph_FromTyped(InterfaceGraph g);

/*variables used in the spliteability function*/
float InterfaceGraph_Density(InterfaceGraph g);
int InterfaceGraph_Size(InterfaceGraph g);
int InterfaceGraph_StronglyConnectedComponents(InterfaceGraph g);
float InterfaceGraph_Grounded(InterfaceGraph g);



void InterfaceGraph_ToFile(FILE *f, InterfaceGraph g);


void InterfaceGraph_Main(Operator *op, char *file);

#endif
