
#ifndef __TRAINING_H
#define __TRAINING_H


#include "ff.h"
#include "inst_pre.h"
#include "Partition.h"
#include "SplitDomain.h"
#include "BlackBoxActionList.h"
#include "HeuristicMeasureList.h"


#define TIMEOUT 10000
#define TRAINING_SET_MAX_SIZE 10
#define TRAINING_MEASURES_SIZE 4


typedef struct _Training{

	char *script_run_path;
	char *planner_path;
	char *bench_path;

	char *domain;
	char domain_path[150];
	char domain_pddl_path[200];

	char set_path[200];

	char *problem;
	char problem_pddl_path[200];

	char result_path[200];


	BlackBoxActionList bba_list;
	SplitActionList sa_list;

	PartitionList generated_partition;


	char *training_set[TRAINING_SET_MAX_SIZE];
	int training_set_size;

	char training_log_path_totaltime[160];
	char training_log_path_searchtime[160];
	char training_log_path_expanded[160];
	char training_log_path_operator[160];


} _Training, *Training;




Training Training_Init(char* script_run_path, char *planner_path, char *bench_path, char *domain);
void Training_Neighbour(Training t,float gamma);
void Training_GeneratePddl(Training t, Partition p);
void Training_RunPlanner(Training t, char *problem_pddl_path, char *result_path);

/*aux*/
int Training_GetHorizon(PartitionList *succesors, float horizon);
void Training_AddGeneratedPartition(Training t, Partition p);
void Training_AddGeneratedPartitions(Training t, PartitionList *l);



/*training log file*/
HeuristicMeasure Training_FetchMeasures(char *result_path, Partition p);
void Training_WriteMeasures(Training t,Partition p);
void Training_LogFileHeader(Training t);


float Training_Partition(char *domain, Partition p, Training t, int n_tolerance, int *acc_tolerance);
Partition Training_Operator(char *domain, Operator *op, Training t,float gamma, float horizon, float tolerance);
void Training_Domain(char* script_run_path, char *planner_path, char *bench_path, char *domain, float gamma, float horizon, float tolerance);




#endif
