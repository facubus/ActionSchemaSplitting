#ifndef __HEURISTICMEASURELIST_H
#define __HEURISTICMEASURELIST_H

#include "HeuristicMeasure.h"

typedef struct hm_node {
	HeuristicMeasure info;
	struct hm_node *next;
}hm_node;


typedef struct _HeuristicMeasureList{
	hm_node *first;
	hm_node *last;
	int size;
}HeuristicMeasureList;




HeuristicMeasureList HeuristicMeasureList_Create();
void HeuristicMeasureList_Add(HeuristicMeasureList *l, HeuristicMeasure x);
int HeuristicMeasureList_Size(const HeuristicMeasureList *l);
HeuristicMeasure HeuristicMeasureList_Get(const HeuristicMeasureList *l, const int index); /*1<= index<= size*/
HeuristicMeasure HeuristicMeasureList_GetLast(const HeuristicMeasureList *l);

void HeuristicMeasureList_Print(const HeuristicMeasureList *l);

#endif
