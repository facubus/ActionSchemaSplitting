#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#include "Training.h"

#include "PartitionList.h"
#include "QuotientGraph.h"
#include "ServicesDomain.h"


#include "SplitAction.h"
#include "Sequentialization.h"
#include "SplitOutput.h"
#include "FloatList.h"


/*aux function to sort the training set by harndness*/
static int compare (const void * a, const void * b){
	/* The pointers point to offsets into "array", so we need to dereference them to get at the strings. */
	return strcmp (*(const char **) a, *(const char **) b);
}

Training Training_Init(char* script_run_path, char *planner_path, char *bench_path, char *domain){


	Training t = (Training) malloc (sizeof(_Training));

	t->script_run_path = script_run_path;
	t->planner_path = planner_path;
	t->bench_path = bench_path;
	t->domain = domain;


	/*build domain path*/
	strcpy(t->domain_path,t->bench_path);
	strcat(t->domain_path,"/");
	strcat(t->domain_path,domain);
	strcat(t->domain_path,"/");

	/*build set path*/
	strcpy(t->set_path, t->domain_path);
	strcat(t->set_path,"training/set/");

	/*partition list generated*/
	t->generated_partition = PartitionList_Create();

	/*sort the training set directory and obtaining its size*/
	DIR *dir;
	struct dirent *ent;
	dir = opendir (t->set_path);
	int i=0;
	while((ent = readdir (dir)) != NULL){
		if((strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0)){
			t->training_set[i] = ent->d_name;
			i++;
		}
	}
	t->training_set_size = i;
	qsort (t->training_set, i, sizeof (const char *), compare);
	/*closedir (dir);*/


	/*build training log path for each measure*/
	strcpy(t->training_log_path_totaltime, t->bench_path);
	strcat(t->training_log_path_totaltime, domain);
	strcat(t->training_log_path_totaltime, "/training/training_log_totaltime.txt");

	strcpy(t->training_log_path_searchtime, t->bench_path);
	strcat(t->training_log_path_searchtime, domain);
	strcat(t->training_log_path_searchtime, "/training/training_log_searchtime.txt");

	strcpy(t->training_log_path_expanded, t->bench_path);
	strcat(t->training_log_path_expanded, domain);
	strcat(t->training_log_path_expanded, "/training/training_log_expanded.txt");

	strcpy(t->training_log_path_operator, t->bench_path);
	strcat(t->training_log_path_operator, domain);
	strcat(t->training_log_path_operator, "/training/training_log_operator.txt");

	return t;
}


void Training_Neighbour(Training t,float gamma){
	t->bba_list = BlackBoxActionList_Create();
	t->sa_list = SplitActionList_Create();

	int i, n = ServicesDomain_GetAmountActions();
	for(i = 1; i <=n ; i++){
		Operator *op = ServicesDomain_GetAction(i);
		BlackBoxAction a = BlackBoxAction_Create(op);
		Partition q = Partition_UnSplitPartition(a,gamma);
		QuotientGraph g = QuotientGraph_Create(q);
		Sequentialization seq = Sequentialization_Create(q,&g);
		Sequentialization_PutTokens(seq);
		SplitAction neighbour = SplitAction_Init(seq);

		BlackBoxActionList_Add(&t->bba_list, a);
		SplitActionList_Add(&t->sa_list, neighbour);
	}

}

void Training_GeneratePddl(Training t, Partition p){

	int i, n = ServicesDomain_GetAmountActions();
	SplitDomain split_domain = SplitDomain_Empty();

	for(i = 1; i <=n ; i++){
		Operator *other = ServicesDomain_GetAction(i);

		if (ServicesDomain_GetActionNumber(other) != ServicesDomain_GetActionNumber(p->op)){
			SplitAction split_action = SplitActionList_Get(&t->sa_list, i);
			SplitDomain_AddSplitAction(split_domain, split_action);

		}else{

			QuotientGraph g = QuotientGraph_Create(p);
			Sequentialization seq = Sequentialization_Create(p,&g);
			Sequentialization_PutTokens(seq);

			SplitDomain_AddSplitAction(split_domain, SplitAction_Init(seq));
		}
	}

	/* build the domain path associated to the partition: "../bench/domain/training/operator/domain_level_seq.pddl" */
	strcpy(t->domain_pddl_path,t->bench_path);
	strcat(t->domain_pddl_path,t->domain);
	strcat(t->domain_pddl_path,"/training/");
	strcat(t->domain_pddl_path,p->op->name);
	mkdir(t->domain_pddl_path, 0700);
	strcat(t->domain_pddl_path,"/domain_");

	/*append level*/
	char str_level[4];
	sprintf(str_level, "%d", p->level);
	strcat(t->domain_pddl_path,str_level);
	strcat(t->domain_pddl_path,"_");


	/*append seq*/
	char str_seq[4];
	sprintf(str_seq, "%d", p->seq);
	strcat(t->domain_pddl_path,str_seq);

	strcat(t->domain_pddl_path,".pddl");

	/*write the file*/
	SplitOutput_SplitDomain(t->domain_pddl_path, split_domain);

}



void Training_RunPlanner(Training t, char *problem_pddl_path, char *result_path){

	char concat[5*200];
	strcpy(concat,t->script_run_path);
	strcat(concat," ");

	strcat(concat,t->planner_path);
	strcat(concat," ");

	strcat(concat,t->domain_pddl_path);
	strcat(concat," ");


	strcat(concat,problem_pddl_path);
	strcat(concat," ");

	strcat(concat,result_path);


	/*printf("%s\n", concat);*/
	if (system(concat)){}
	/*
	*/


}




HeuristicMeasure Training_FetchMeasures(char *result_path, Partition p){
	FILE *f;
	char line[20];

	char time_path[200];
	strcpy(time_path, result_path);
	strcat(time_path, "time");


	/*printf("FOPEN %s\n\n", result_path);*/
	f = fopen (time_path, "r");

	/*recollect data and build a measure for it*/
	float total_time;
	if (fgets(line, 10, f) != NULL) sscanf (line, "%f", &total_time);

	float search_time;
	if (fgets(line, 10, f) != NULL) sscanf (line, "%f", &search_time);

	float expanded;
	if (fgets(line, 10, f) != NULL) sscanf (line, "%f", &expanded);

	float operators;
	if (fgets(line, 10, f) != NULL) sscanf (line, "%f", &operators);


	fclose(f);

	HeuristicMeasure hm = HeuristicMeasure_Create(total_time, search_time, expanded, operators);
	Partition_AddHeuristicMeasure(p,hm);
	return hm;


}


void Training_WriteMeasures(Training t,Partition p){

	FILE *f1 = fopen(t->training_log_path_totaltime, "a");
	FILE *f2 = fopen(t->training_log_path_searchtime, "a");
	FILE *f3 = fopen(t->training_log_path_expanded, "a");
	FILE *f4 = fopen(t->training_log_path_operator, "a");

	if (HeuristicMeasureList_Size(&p->measure) == 1){
		fprintf(f1, "%s %d_%d %.2f", p->op->name, p->level, p->seq, p->tradeoff);
		fprintf(f1, " %.2f", HeuristicMeasureList_Get(&p->measure,1)->total_time);

		fprintf(f2, "%s %d_%d %.2f", p->op->name, p->level, p->seq, p->tradeoff);
		fprintf(f2, " %.2f", HeuristicMeasureList_Get(&p->measure,1)->search_time);


		fprintf(f3, "%s %d_%d %.2f", p->op->name, p->level, p->seq, p->tradeoff);
		fprintf(f3, " %d", HeuristicMeasureList_Get(&p->measure,1)->expanded);

		fprintf(f4, "%s %d_%d %.2f", p->op->name, p->level, p->seq, p->tradeoff);
		fprintf(f4, " %d", HeuristicMeasureList_Get(&p->measure,1)->operators);


	}else if(HeuristicMeasureList_Size(&p->measure) == t->training_set_size){
		fprintf(f1, " %.2f\n", HeuristicMeasureList_GetLast(&p->measure)->total_time);
		fprintf(f2, " %.2f\n", HeuristicMeasureList_GetLast(&p->measure)->search_time);
		fprintf(f3, " %d\n", HeuristicMeasureList_GetLast(&p->measure)->expanded);
		fprintf(f4, " %d\n", HeuristicMeasureList_GetLast(&p->measure)->operators);

	}else{
		fprintf(f1, " %.2f", HeuristicMeasureList_GetLast(&p->measure)->total_time);
		fprintf(f2, " %.2f", HeuristicMeasureList_GetLast(&p->measure)->search_time);
		fprintf(f3, " %d", HeuristicMeasureList_GetLast(&p->measure)->expanded);
		fprintf(f4, " %d", HeuristicMeasureList_GetLast(&p->measure)->operators);

	}
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
}


void Training_LogFileHeader(Training t){
	FILE *f1 = fopen(t->training_log_path_totaltime, "a");
	FILE *f2 = fopen(t->training_log_path_searchtime, "a");
	FILE *f3 = fopen(t->training_log_path_expanded, "a");
	FILE *f4 = fopen(t->training_log_path_operator, "a");


	fprintf(f1, "Operator Partition TradeOff");
	fprintf(f2, "Operator Partition TradeOff");
	fprintf(f3, "Operator Partition TradeOff");
	fprintf(f4, "Operator Partition TradeOff");

	/*print problem name of the training set*/
	int i;
	for (i=0; i<t->training_set_size; i++){
			fprintf(f1, " %s",t->training_set[i]);
			fprintf(f2, " %s",t->training_set[i]);
			fprintf(f3, " %s",t->training_set[i]);
			fprintf(f4, " %s",t->training_set[i]);


	}
	fprintf(f1, "\n");
	fprintf(f2, "\n");
	fprintf(f3, "\n");
	fprintf(f4, "\n");


	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);



}





int Training_GetHorizon(PartitionList *succesors, float horizon){
	int n = floor(PartitionList_Size(succesors) * horizon) + 1;
	n = n > PartitionList_Size(succesors) ?  PartitionList_Size(succesors) : n;
	if (n >= 5)
		return n;
	else
		if (PartitionList_Size(succesors)<5)
			return PartitionList_Size(succesors);
		else return 5;
}

void Training_AddGeneratedPartition(Training t, Partition p){
	PartitionList_Add(&t->generated_partition,p);
}
void Training_AddGeneratedPartitions(Training t, PartitionList *l){
	int i,n = PartitionList_Size(l);
	for(i=1; i<=n; i++){
		PartitionList_Add(&t->generated_partition, PartitionList_Get(l,i));
	}
}








float Training_Partition(char *domain, Partition p, Training t, int n_tolerance, int *acc_tolerance){


	Training_GeneratePddl(t, p);
	float acc_avg_time = 0.0;

	int i;
	for (i=0; i<t->training_set_size; i++){

		/*build problem.pddl path*/
		char problem_pddl_path[200];
		strcpy(problem_pddl_path,t->set_path);
		strcat(problem_pddl_path, t->training_set[i]);

		/*build result path*/
		char result_path[200];
		strcpy(result_path,t->domain_path);
		strcat(result_path, "training/");
		strcat(result_path, p->op->name);
		strcat(result_path, "/result_");

		/*append level*/
		char str_level[4];
		sprintf(str_level, "%d", p->level);
		strcat(result_path,str_level);
		strcat(result_path, "_");


		/*append seq*/
		char str_seq[4];
		sprintf(str_seq, "%d", p->seq);
		strcat(result_path,str_seq);
		strcat(result_path, "/");
		strcat(result_path, t->training_set[i]);
		strcat(result_path, "/");


		Training_RunPlanner(t,problem_pddl_path,result_path);
		HeuristicMeasure hm = Training_FetchMeasures(result_path,p);
		Training_WriteMeasures(t,p);


		if (hm->total_time == -1){ /*if timeout*/
			acc_avg_time += TIMEOUT;
			if (*acc_tolerance == n_tolerance) return -1; else *acc_tolerance = *acc_tolerance + 1;
		} else{
			acc_avg_time += hm->total_time;
		}

		/*
		 */
	}

	return (acc_avg_time/t->training_set_size);

}


Partition Training_Operator(char *domain, Operator *op, Training t,float gamma, float horizon, float tolerance){

	BlackBoxAction a = 	BlackBoxActionList_Get(&t->bba_list, ServicesDomain_GetActionNumber(op));
	Partition p = Partition_AtomPartition(a, gamma);
	QuotientGraph g = QuotientGraph_Create(p);

	Training_AddGeneratedPartition(t,p);

	PartitionList succesors = QuotientGraph_Succesors(p, &g);
	PartitionList_LevelSeqTradeoff(&succesors,op,2,gamma,a->N1,a->N2); /* Set level, seq & tradeoff of the succesors*/
	PartitionList sort_succesors = PartitionList_Sort(&succesors); /* Sort succesors by TradeOff */
	Training_AddGeneratedPartitions(t,&succesors);


	int n_tolerance = floor(t->training_set_size * tolerance);
	int acc_tolerance = 0;
	Partition best_partition = p;
	float best_cost = Training_Partition(domain,p,t,n_tolerance, &acc_tolerance);

	int i, n_horizon = Training_GetHorizon(&succesors, horizon);
	int level = 2;
	n_tolerance = floor(n_horizon * t->training_set_size * tolerance);
	acc_tolerance = 0;

	while (PartitionList_Size(&succesors) != 0){

		/*training succesors*/
		FloatList succesors_cost = FloatList_Create();
		for(i=1; i<=n_horizon; i++){
			Partition suc = PartitionList_Get(&sort_succesors,i);
			if (PartitionList_Size(&succesors) == 1) n_tolerance = t->training_set_size; /*case Trivial Split level*/
			float avg_time = Training_Partition(domain, suc, t, n_tolerance, &acc_tolerance);
			if (avg_time != -1) FloatList_Add(&succesors_cost, avg_time);
			else break;
		}

		/*obtain the succesor with min avg time*/
		int imin = FloatList_Minimum(&succesors_cost, 0);
		float suc_min_cost = FloatList_Get(&succesors_cost, imin);

		/*compare the succesor with min avg time againts partial best partition found*/
		if(suc_min_cost <= best_cost){
			best_partition = PartitionList_Get(&sort_succesors,imin);
			best_cost = suc_min_cost;
		}

		/*set next iteration*/
		p = PartitionList_Get(&sort_succesors,imin);
		g = QuotientGraph_Create(p);

		level++;

		succesors = QuotientGraph_Succesors(p, &g);
		PartitionList_LevelSeqTradeoff(&succesors,op,level,gamma,a->N1,a->N2);
		sort_succesors = PartitionList_Sort(&succesors); /* Sort succesors by TradeOff */
		Training_AddGeneratedPartitions(t,&succesors);

		n_horizon = Training_GetHorizon(&succesors, horizon);
		tolerance += 0.1;
		n_tolerance = floor(n_horizon * t->training_set_size * tolerance);
		acc_tolerance = 0;
	}
	return best_partition;

}



void Training_Domain(char* script_run_path, char *planner_path, char *bench_path, char *domain, float gamma, float horizon, float tolerance){
	Training t = Training_Init(script_run_path, planner_path, bench_path, domain);

	Training_LogFileHeader(t);
	Training_Neighbour(t,gamma);

	SplitDomain split_domain = SplitDomain_Empty();

	int i,n = ServicesDomain_GetAmountActions();
	for(i=1;i<=n;i++){
		Operator *op = ServicesDomain_GetAction(i);
		Partition p = 	Training_Operator(domain, op,t, gamma, horizon,tolerance);
		QuotientGraph g = QuotientGraph_Create(p);
		Sequentialization seq = Sequentialization_Create(p,&g);
		Sequentialization_PutTokens(seq);


		SplitAction split_action = (SplitAction) malloc (sizeof(_SplitAction));
		split_action->subactions = seq;

		SplitDomain_AddSplitAction(split_domain,split_action);
	}

	/*write split domain pddl file*/
	char split_pddl[160];
	strcpy(split_pddl, t->bench_path);
	strcat(split_pddl, domain);
	strcat(split_pddl, "/training/training_domain.pddl");
	SplitOutput_SplitDomain(split_pddl, split_domain);

	/*write unsplit log file*/
	char split_log[160];
	strcpy(split_log, t->bench_path);
	strcat(split_log, domain);
	strcat(split_log, "/training/training_unsplit.txt");
	SplitOutput_SplitLog(split_log, split_domain);


}

















