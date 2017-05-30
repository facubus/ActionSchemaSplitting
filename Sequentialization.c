#include "ff.h"
#include "inst_pre.h"

#include "Sequentialization.h"
#include "ServicesDomain.h"
#include "SubPartition.h"



Sequentialization Sequentialization_Empty(Operator *op){

	/*memory allocation*/
	Sequentialization seq = (Sequentialization) malloc (sizeof(_Sequentialization));
	seq->sequence = SubPartitionList_Create();
	seq->operator = op;
	return seq;
}


Sequentialization Sequentialization_Create(Partition p, QuotientGraph *g){
/* g must be the quotient graph of partition p */

	int n = Partition_GetSize(p);
	Sequentialization seq = Sequentialization_Empty(p->op);

	IntegerList except = IntegerList_Create();
	do{

		IntegerList nodes = QuotientGraph_WithoutIncomingEdgeList(g,&except);
		QuotientGraph_DeleteOutcomingEdgeList(g,&nodes);

		SubPartitionList list = SubPartitionList_Create();
		int i, m = IntegerList_Size(&nodes);
		for(i = 1; i <= m; i++){
			SubPartitionList_Add(&list, Partition_GetSubPartition(p,IntegerList_Get(&nodes,i)));
		}

		list = SubPartitionList_Sort(&list);
		Sequentialization_AddSubPartitionList(seq, &list);

	}while(Sequentialization_Size(seq) < n);

	return seq;
}


int Sequentialization_Size(const Sequentialization seq){
	return SubPartitionList_Size(&seq->sequence);
}

SubPartition Sequentialization_GetPosition(const Sequentialization seq, int index){
	return SubPartitionList_Get(&seq->sequence, index);
}
SubPartition Sequentialization_GetFirst(const Sequentialization seq){
	return SubPartitionList_Get(&seq->sequence, 1);
}
SubPartition Sequentialization_GetLast(const Sequentialization seq){
	return SubPartitionList_Get(&seq->sequence, SubPartitionList_Size(&seq->sequence));
}

Bool Sequentialization_isSingleton(const Sequentialization seq){
	return SubPartitionList_Size(&seq->sequence) == 1;
}


void Sequentialization_AddSubPartition(Sequentialization seq, SubPartition sp){
	SubPartitionList_Add(&seq->sequence,sp);
}
void Sequentialization_AddSubPartitionList(Sequentialization seq, SubPartitionList *list){
	int i, n = SubPartitionList_Size(list);
	for(i = 1; i <= n; i++){
		SubPartitionList_Add(&seq->sequence, SubPartitionList_Get(list,i));
	}
}


Bool Sequentialization_Before(Sequentialization seq, int var, int position){
	int i;
	for(i = 1; i < position; i++){
		if (SubPartition_isVar(Sequentialization_GetPosition(seq,i),var)) return TRUE;
	}
	return FALSE;
}
Bool Sequentialization_After(Sequentialization seq, int var, int position){
	int i, n = Sequentialization_Size(seq);
	for(i = position + 1; i <= n; i++){
		if (SubPartition_isVar(Sequentialization_GetPosition(seq,i),var)) return TRUE;
	}
	return FALSE;
}


void Sequentialization_Print(Sequentialization seq){
	printf("------------------Sequentilization------------------\n");
	SubPartitionList_Print(&seq->sequence);

}


void Sequentialization_PutTokenProcnone(Sequentialization seq){
	if (Sequentialization_isSingleton(seq)){
		/* at first: procnone -> pre */
		SplitToken procnone = SplitToken_Create(1,-1,-1,-1,1);
		SubPartition_AddSplitTokenPre(Sequentialization_GetFirst(seq),procnone);
	}else{
		SplitToken procnone = SplitToken_Create(1,-1,-1,-1,1);
		SplitToken not_procnone = SplitToken_Create(1,-1,-1,-1,0);

		/* at first: procnone -> pre,del */
		SubPartition_AddSplitTokenPre(Sequentialization_GetFirst(seq),procnone);
		SubPartition_AddSplitTokenEff(Sequentialization_GetFirst(seq),not_procnone);

		/* at last: procnone -> add*/
		SubPartition_AddSplitTokenEff(Sequentialization_GetLast(seq),procnone);
	}
}

void Sequentialization_PutTokenDo(Sequentialization seq){
	if (!Sequentialization_isSingleton(seq)){

		int id_operator = ServicesDomain_GetActionNumber(seq->operator);

		/* at first: add => do_2*/
		SplitToken do_2 = SplitToken_Create(2,id_operator,2,-1,1);
		SubPartition_AddSplitTokenEff(Sequentialization_GetFirst(seq), do_2);

		int i,n = Sequentialization_Size(seq);
		for(i = 2; i <= n-1; i++){
			/*subaction_i: pre => do_i  add => do_i+1  del => do_i*/
			SplitToken do_i = SplitToken_Create(2,id_operator,i,-1,1);
			SubPartition_AddSplitTokenPre(Sequentialization_GetPosition(seq,i), do_i);

			SplitToken do_ii = SplitToken_Create(2,id_operator,i+1,-1,1);
			SubPartition_AddSplitTokenEff(Sequentialization_GetPosition(seq,i), do_ii);

			SplitToken not_do_i = SplitToken_Create(2,id_operator,i,-1,0);
			SubPartition_AddSplitTokenEff(Sequentialization_GetPosition(seq,i), not_do_i);
		}


		/*last: pre => do_n  del => do_n*/
		SplitToken do_n = SplitToken_Create(2,id_operator,n,-1,1);
		SubPartition_AddSplitTokenPre(Sequentialization_GetLast(seq), do_n);

		SplitToken not_do_n = SplitToken_Create(2,id_operator,n,-1,0);
		SubPartition_AddSplitTokenEff(Sequentialization_GetLast(seq), not_do_n);

	}
}

void Sequentialization_PutTokenArg(Sequentialization seq){
	if (!Sequentialization_isSingleton(seq)){
		int i,j,n = Sequentialization_Size(seq);
		for(i = 1; i <= n; i++){
			SubPartition sp = Sequentialization_GetPosition(seq,i);
			int m = SubPartition_GetInterfaceSize(sp);
			for(j = 1; j <= m; j++){
				int var = SubPartition_GetVar(sp,j);
				Bool before = Sequentialization_Before(seq,var,i);
				Bool after = Sequentialization_After(seq,var,i);
				if (before && after){
					/*arg(v) -> pre*/
					SubPartition_AddSplitTokenPre(sp, SplitToken_Create(3,-1,-1,var,1));
				}
				else if (before && !after){
					/*arg(v)->pre*/
					SubPartition_AddSplitTokenPre(sp, SplitToken_Create(3,-1,-1,var,1));
					/*arg(v)->del*/
					SubPartition_AddSplitTokenEff(sp, SplitToken_Create(3,-1,-1,var,0));
				}
				else if(!before && after){
					/*arg(v) ->add*/
					SubPartition_AddSplitTokenEff(sp, SplitToken_Create(3,-1,-1,var,1));

					/********* !!!!! Decoration_AddSplitToken(dec,token); !!!!! ********/

				}else{
					/*skip*/
				}
			}
		}
	}
}


void Sequentialization_ClearTokens(Sequentialization seq){
	int i,n = Sequentialization_Size(seq);
	for(i = 1; i <= n; i++){
		SubPartition sp = Sequentialization_GetPosition(seq,i);
		SubPartition_ClearSplitTokenPre(sp);
		SubPartition_ClearSplitTokenEff(sp);

	}
}


void Sequentialization_PutTokens(Sequentialization seq){
	Sequentialization_ClearTokens(seq);
	Sequentialization_PutTokenProcnone(seq);
	Sequentialization_PutTokenDo(seq);
	Sequentialization_PutTokenArg(seq);
/*
*/
}







