#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>


#include "HeuristicMeasure.h"


HeuristicMeasure HeuristicMeasure_Create(float total_time, float search_time, int expanded, int operators){
	HeuristicMeasure m = (HeuristicMeasure) malloc (sizeof(_HeuristicMeasure));
	m->total_time = total_time;
	m->search_time = search_time;
	m->expanded = expanded;
	m->operators = operators;
	return m;
}



void HeuristicMeasure_Print(HeuristicMeasure m){
	printf("%.2f %d %d\n", m->search_time, m->expanded, m->operators);
}

















