
#ifndef __DECORATION_H
#define __DECORATION_H


#include "Sequentialization.h"
#include "SplitTokenList.h"


typedef struct _Decoration{
	SplitTokenList tokens;

} _Decoration, *Decoration;


Decoration Decoration_Init();

int Decoration_Size(Decoration dec);
void Decoration_AddSplitToken(Decoration dec, SplitToken token);
SplitToken Decoration_GetSplitToken(Decoration dec, int index); /*1<=index<=size*/
Bool Decoration_ContainsToken(Decoration dec, SplitToken token);



void Decoration_PutTokenProcnone(Sequentialization seq, Decoration dec);
void Decoration_PutTokenDo(Sequentialization seq, Decoration dec, int action_number);
void Decoration_PutTokenArg(Sequentialization seq, Decoration dec);





#endif
