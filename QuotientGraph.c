#include "ff.h"
#include "inst_pre.h"
#include "QuotientGraph.h"



QuotientGraph QuotientGraph_Create(Partition p){
	QuotientGraph g;
	g.size = Partition_GetSize(p);

	/* Relation memory allocate */
	g.relation = (Bool**) calloc(g.size, sizeof(Bool*));
	int i,j;
	for(i = 0; i < g.size; i++){
		g.relation[i] = (Bool*) calloc(g.size, sizeof(Bool));
	}

	/* computing the quotient graph of the partition */
	for(i = 1; i <= g.size; i++){
		for(j = 1 ; j <= g.size; j++){
			if (i!=j){
				g.relation[i-1][j-1] = SubPartition_Depends(Partition_GetSubPartition(p,i), Partition_GetSubPartition(p,j));
			}
		}
	}


	/* Transitive Clousure memory allocate */
	g.transitive_clousure = (Bool**) calloc(g.size, sizeof(Bool*));
	for(i = 0; i < g.size; i++){
		g.transitive_clousure[i] = (Bool*) calloc(g.size, sizeof(Bool));
	}

	/* Transitive Clousure := Relation */
	for (j = 0; j < g.size; j++){
		for (i = 0; i < g.size; i++){
			g.transitive_clousure[i][j] = g.relation[i][j];
		}
	}

	/* computing the Transitive Clousure of the Relation */
	int k;
	for(k = 0; k < g.size; k++){
		for(j = 0; j < g.size; j++){
			for(i = 0; i < g.size; i++){
				if(!g.transitive_clousure[i][j]){
					g.transitive_clousure[i][j]= g.transitive_clousure[i][k] && g.transitive_clousure[k][j];
				}
			}
		}
	}

	return g;
}


Bool QuotientGraph_AreConnected(QuotientGraph *g, int i, int j){ /*1 <= i!=j <size(p)*/
/* if i --*--> j in the transitive closure */
	return g->transitive_clousure[i-1][j-1];
}

Bool QuotientGraph_AreMergeable(QuotientGraph *g, int i,int j){ /*1 <= i!=j <size(p)*/
	int k;
	for(k = 1; k <= g->size; k++){
		if (k!=i && k!=j){
			if ((!QuotientGraph_AreConnected(g,i,k) || !QuotientGraph_AreConnected(g,k,j)) &&
					(!QuotientGraph_AreConnected(g,j,k) || !QuotientGraph_AreConnected(g,k,i))){
				continue;
			}else{
				return FALSE;
			}
		}
	}
	return TRUE;
}

Bool QuotientGraph_isCyclic(const QuotientGraph *g){
	int i;
	for(i = 0; i < g->size; i++){
		if(g->transitive_clousure[i][i] == TRUE) return TRUE;
	}
	return FALSE;

}

PartitionList QuotientGraph_Succesors(const Partition p, QuotientGraph *g){
/* g must be the quotient graph of the partition p */
	PartitionList l = PartitionList_Create();
	int i,j;
	for(i = 1; i <= g->size; i++){
			for(j = i+1 ; j <= g->size; j++){
				if (QuotientGraph_AreMergeable(g,i,j)){
					PartitionList_Add(&l, Partition_Merge(p,i,j));
				}
			}
	}

	return l;
}

sc_result_t *QuotientGraph_StronglyConnectedComponents(const QuotientGraph *g){

	dgraph_t *gg = dgraph_blank(g->size);
	int i,j;
	for (i = 0; i < g->size; i++){
		for (j = 0; j < g->size; j++){
			if (g->relation[i][j] == TRUE){
				dgraph_add_new_edge(gg,i,j);
			}
		}
	}

	return sc(gg);

}


Partition QuotientGraph_Collapse(const Partition p, const QuotientGraph *g){
	sc_result_t *scc_result = QuotientGraph_StronglyConnectedComponents(g);
	Partition collpased = Partition_Create();
	int i,j;
	for(i = 0; i < scc_result->n_sets; i++){
		SubPartition scc = SubPartition_Empty();
		for(j = scc_result->sets_s[i]; j <= scc_result->sets_f[i]; j++){
			int k = scc_result->vertices[j] + 1;
			SubPartition sp = Partition_GetSubPartition(p,k);
			scc = SubPartition_Merge(scc,sp);
		}
		Partition_AddSubPartition(collpased, scc);
	}
	return collpased;

}

Bool QuotientGraph_WithoutIncomingEdge(const QuotientGraph *g, int j){ /*1<=j<=size(g)*/
	int i;
	for(i = 0; i < g->size; i++){
		if (g->relation[i][j-1] == TRUE) return FALSE;
	}
	return TRUE;
}

IntegerList QuotientGraph_WithoutIncomingEdgeList(const QuotientGraph *g, IntegerList *except){
	IntegerList list = IntegerList_Create();
	int j;
	for(j = 1; j <= g->size; j++){
		if (!IntegerList_Contains(except,j) && QuotientGraph_WithoutIncomingEdge(g,j)){
			IntegerList_Add(&list,j);
			IntegerList_Add(except,j);
		}
	}
	return list;
}

void QuotientGraph_DeleteOutcomingEdge(QuotientGraph *g, int j){ /*1<=j<=size(g)*/
	int k;
	for(k = 0; k < g->size; k++){
		g->relation[j-1][k] = FALSE;
	}
}
void QuotientGraph_DeleteOutcomingEdgeList(QuotientGraph *g, IntegerList *nodes){
	int i, n = IntegerList_Size(nodes);
	for (i=1; i<=n; i++){
		QuotientGraph_DeleteOutcomingEdge(g, IntegerList_Get(nodes,i));
	}
}

void QuotientGraph_Print(const QuotientGraph *g){
	int i,j;
	for(i = 0; i < g->size; i++){
		for(j = 0; j< g->size; j++){
			printf("%d", g->relation[i][j]);
	 }
	printf("\n");
	}
}

