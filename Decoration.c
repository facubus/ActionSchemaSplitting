#include "ff.h"
#include "inst_pre.h"

#include "Decoration.h"
#include "ServicesDomain.h"


Decoration Decoration_Init(){
	Decoration dec = (Decoration) malloc (sizeof(_Decoration));
	dec->tokens = SplitTokenList_Create();
	return dec;
}

int Decoration_Size(Decoration dec){
	return SplitTokenList_Size(&dec->tokens);
}
void Decoration_AddSplitToken(Decoration dec, SplitToken token){
	if (!Decoration_ContainsToken(dec,token))
		SplitTokenList_Add(&dec->tokens,token);
}

Bool Decoration_ContainsToken(Decoration dec, SplitToken token){

	int i, n = Decoration_Size(dec);
	for(i = 1; i <= n; i++){
		SplitToken t = Decoration_GetSplitToken(dec,i);
		if (token->type == t->type && token->var == t->var) return TRUE;
	}
	return FALSE;
}


SplitToken Decoration_GetSplitToken(Decoration dec, int index){
	return SplitTokenList_Get(&dec->tokens, index);
}





void Decoration_PutTokenProcnone(Sequentialization seq, Decoration dec){

	BlackBox procnone = BlackBox_CreateFromSplitToken(SplitToken_Create(1,0,0,0,1));
	if (Sequentialization_isSingleton(seq)){
		/* first: procnone -> pre */
		SubPartition_AddPreBlackBox(Sequentialization_GetFirst(seq), procnone);
	}else{
		/* first: procnone -> pre,del */
		SubPartition_AddPreBlackBox(Sequentialization_GetFirst(seq), procnone);

		BlackBox not_procnone = BlackBox_CreateFromSplitToken(SplitToken_Create(1,0,0,0,0));
		SubPartition_AddEffBlackBox(Sequentialization_GetFirst(seq), not_procnone);

		/* last: procnone -> add*/
		SubPartition_AddEffBlackBox(Sequentialization_GetLast(seq), procnone);
	}
}




void Decoration_PutTokenDo(Sequentialization seq, Decoration dec, int action_number){
	if (!Sequentialization_isSingleton(seq)){

		/*first: add => do_2*/
		BlackBox do_2_add = BlackBox_CreateFromSplitToken(SplitToken_Create(2,action_number,2,0,1));
		SubPartition_AddEffBlackBox(Sequentialization_GetFirst(seq), do_2_add);

		int i,n = Sequentialization_Size(seq);
		for(i = 2; i <= n-1; i++){
			/*subaction_i: pre => do_i  add => do_i+1  del => do_i*/
			BlackBox do_i = BlackBox_CreateFromSplitToken(SplitToken_Create(2,action_number,i,0,1));
			SubPartition_AddPreBlackBox(Sequentialization_GetPosition(seq,i), do_i);

			BlackBox do_ii = BlackBox_CreateFromSplitToken(SplitToken_Create(2,action_number,i+1,0,1));
			SubPartition_AddEffBlackBox(Sequentialization_GetPosition(seq,i), do_ii);

			BlackBox not_do_i = BlackBox_CreateFromSplitToken(SplitToken_Create(2,action_number,i,0,0));
			SubPartition_AddEffBlackBox(Sequentialization_GetPosition(seq,i), not_do_i);
		}


		/*last: pre => do_n  del => do_n*/
		BlackBox do_n = BlackBox_CreateFromSplitToken(SplitToken_Create(2,action_number,n,0,1));
		SubPartition_AddPreBlackBox(Sequentialization_GetLast(seq), do_n);

		BlackBox not_do_n = BlackBox_CreateFromSplitToken(SplitToken_Create(2,action_number,n,0,0));
		SubPartition_AddEffBlackBox(Sequentialization_GetLast(seq), not_do_n);


	}
}


void Decoration_PutTokenArg(Sequentialization seq, Decoration dec){
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
					BlackBox arg = BlackBox_CreateFromSplitToken(SplitToken_Create(3,0,0,var,1));
					SubPartition_AddPreBlackBox(sp,arg);
				}else if (before && !after){
					/*arg(v)->pre*/
					BlackBox arg = BlackBox_CreateFromSplitToken(SplitToken_Create(3,0,0,var,1));
					SubPartition_AddPreBlackBox(sp,arg);
					/*arg(v)->del*/
					BlackBox not_arg = BlackBox_CreateFromSplitToken(SplitToken_Create(3,0,0,var,0));
					SubPartition_AddEffBlackBox(sp,not_arg);
				}else if(!before && after){
					/*arg(v) ->add*/
					SplitToken token = SplitToken_Create(3,0,0,var,1);
					BlackBox arg = BlackBox_CreateFromSplitToken(token);
					SubPartition_AddEffBlackBox(sp,arg);
					Decoration_AddSplitToken(dec,token);

				}else{
					/*skip*/
				}

			}
		}
	}
}



