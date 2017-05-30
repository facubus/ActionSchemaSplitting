
#ifndef __SPLITTOKEN_H
#define __SPLITTOKEN_H


typedef struct _SplitToken{

	int type; 		/*1=>procnone 2=>Do 3=>arg*/
	int action;		/*action number for the token do*/
	int subaction;	/*subaction number for the token do*/
	int var; 		/*variable for the token arg*/
	Bool positive;	/*<token> or (not<token>)*/

} _SplitToken, *SplitToken;



SplitToken SplitToken_Create(int type, int action, int subaction, int var, Bool positive);

Bool SplitToken_isProcnone(SplitToken token);
Bool SplitToken_isDo(SplitToken token);
Bool SplitToken_isArg(SplitToken token);

void SplitToken_Destroy(SplitToken token);
void SplitToken_Print(SplitToken token);

#endif
