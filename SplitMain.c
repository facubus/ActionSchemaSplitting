#include "ff.h"
#include "inst_pre.h"
#include "SplitMain.h"
#include "SplitDomain.h"
#include "SplitOutput.h"
#include "Training.h"



void SplitMain_byHeuristicCost(char *split_domain_path,char *split_problem_path,char *visual_graph_path, char *splitted_operator_path,float gamma,int threshold){
	printf("\nRunning Action Schema Splitting with gamma = %.2f and threshold = %d\n", gamma,threshold);

	SplitDomain d = SplitDomain_Create(visual_graph_path,gamma,1,threshold);
	SplitOutput_SplitDomain(split_domain_path, d);
	SplitOutput_SplitProblem(split_problem_path);

	/*
	Splitability_ToVisualGraph(d->splitability,visual_graph_path);
	SplitOutput_SplittedOperators(splitted_operator_path, d);
	*/

}



void SplitMain_byTraining(char *script_run_path, char *planner_path, char *bench_path, char *domain, float gamma, float horizon, float tolerance){
	/*
	 * Training_Domain(script_run_path, planner_path, bench_path, domain, gamma, horizon, tolerance);
	 */
}




void SplitMain(char *split_domain_path,char *split_problem_path,char *visual_graph_path, char *splitted_operator_path, char *gamma, char* threshold){
	SplitMain_byHeuristicCost(split_domain_path, split_problem_path, visual_graph_path,splitted_operator_path, atof(gamma),atoi(threshold));
}





/*
void SplitMain(char *config, char *mode){
	if (strcmp(mode,"h") == 0) {
		printf("by Heuristic Mode\n");
		FILE *f = fopen(config, "r");

		int max_length = 120;
		char *line;
		int length;
		line = (char *) malloc(sizeof(char) * max_length);

		//split domain file
		length = getline(&line, &max_length, f);
		char *split_domain_file = (char *)malloc(sizeof(char) * (length-1));
		memcpy(split_domain_file, line, length-1);

		//split problem file
		length = getline(&line, &max_length, f);
		char *split_problem_file = (char *)malloc(sizeof(char) * (length-1));
		memcpy(split_problem_file, line, length-1);

		//visual interface graph
		length = getline(&line, &max_length, f);
		char *visual_graph_file = (char *)malloc(sizeof(char) * (length-1));
		memcpy(visual_graph_file, line, length-1);

		//split gamma
		length = getline(&line, &max_length, f);
		char *gamma = (char *)malloc(sizeof(char) * (length-1));
		memcpy(gamma, line, length-1);

		//split log file/
		length = getline(&line, &max_length, f);
		char *split_log_file = (char *)malloc(sizeof(char) * (length-1));
		memcpy(split_log_file, line, length-1);


		//split B
		length = getline(&line, &max_length, f);
		char *b = (char *)malloc(sizeof(char) * (length-1));
		memcpy(b, line, length-1);

		fclose(f);

		SplitMain_byHeuristicCost(split_domain_file, split_problem_file, visual_graph_file, gamma);

	}

	if (strcmp(mode,"t") == 0){
		printf("by Training Mode\n");

		FILE *f = fopen(config, "r");
		int max_length = 100;
		char *line;
		int length;
		line = (char *) malloc(sizeof(char) * max_length);

		// run.py file
		length = getline(&line, &max_length, f);
		char *script_run_path = (char *)malloc(sizeof(char) * (length-1));
		memcpy(script_run_path, line, length-1);

		// planner path
		length = getline(&line, &max_length, f);
		char *planner_path = (char *)malloc(sizeof(char) * (length-1));
		memcpy(planner_path, line, length-1);


		// bench_path
		length = getline(&line, &max_length, f);
		char *bench_path = (char *)malloc(sizeof(char) * (length-1));
		memcpy(bench_path, line, length-1);

		// domain folder
		length = getline(&line, &max_length, f);
		char *domain = (char *)malloc(sizeof(char) * (length-1));
		memcpy(domain, line, length-1);


		// split training gamma
		length = getline(&line, &max_length, f);
		char *gamma = (char *)malloc(sizeof(char) * (length-1));
		memcpy(gamma, line, length-1);

		// split training horizon
		length = getline(&line, &max_length, f);
		char *horizon = (char *)malloc(sizeof(char) * (length-1));
		memcpy(horizon, line, length-1);


		// split training tolerance
		length = getline(&line, &max_length, f);
		char *tolerance = (char *)malloc(sizeof(char) * (length-1));
		memcpy(tolerance, line, length-1);

		fclose(f);

		SplitMain_byTraining(script_run_path, planner_path, bench_path, domain, atof(gamma), atof(horizon), atof(tolerance));
	}
}
*/




