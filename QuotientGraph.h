
#ifndef __QUOTIENTGRAPH_H
#define __QUOTIENTGRAPH_H

#include "Partition.h"
#include "PartitionList.h"
#include "dgraph.h"
#include "sc.h"

typedef struct _QuotientGraph{
  Bool **relation;
  Bool **transitive_clousure;
  int size;

} QuotientGraph;



QuotientGraph QuotientGraph_Create(Partition p);



Bool QuotientGraph_AreConnected(QuotientGraph *g,int i,int j); /*1<=i,j<size(p)*/
Bool QuotientGraph_AreMergeable(QuotientGraph *g, int i,int j); /*1<=i,j<size(p)*/

Bool QuotientGraph_isCyclic(const QuotientGraph *g);

PartitionList QuotientGraph_Succesors(const Partition p, QuotientGraph *g);

sc_result_t *QuotientGraph_StronglyConnectedComponents(const QuotientGraph *g);
Partition QuotientGraph_Collapse(const Partition p, const QuotientGraph *g);


Bool QuotientGraph_WithoutIncomingEdge(const QuotientGraph *g, int j); /*1<=j<=size(g)*/
IntegerList QuotientGraph_WithoutIncomingEdgeList(const QuotientGraph *g, IntegerList *except);

void QuotientGraph_DeleteOutcomingEdge(QuotientGraph *g, int j); /*1<=j<=size(g)*/
void QuotientGraph_DeleteOutcomingEdgeList(QuotientGraph *g, IntegerList *nodes);

void QuotientGraph_Print(const QuotientGraph *g);







#endif
