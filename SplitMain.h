#ifndef __SPLITMAIN_H
#define __SPLITMAIN_H



void SplitMain_byHeuristicCost(char *split_domain_path,char *split_problem_path,char *visual_graph_path, char *splitted_operator_path, float gamma,int threshold);
void SplitMain_byTraining(char *script_run_path, char *planner_path, char *bench_path, char *domain, float gamma, float horizon, float tolerance);
void SplitMain(char *split_domain_path,char *split_problem_path,char *visual_graph_path, char *splitted_operator_path, char *gamma, char* threshold);




#endif
