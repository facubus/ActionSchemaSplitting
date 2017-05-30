
#ifndef __HEURISTICMEASURE_H
#define __HEURISTICMEASURE_H



typedef struct _HeuristicMeasure{
	float total_time;
	float search_time;
	int expanded;
	int operators;

} _HeuristicMeasure, *HeuristicMeasure;




HeuristicMeasure HeuristicMeasure_Create(float total_time, float search_time, int expanded, int operators);
void HeuristicMeasure_Print(HeuristicMeasure m);


#endif
