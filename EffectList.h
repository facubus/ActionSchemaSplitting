
#ifndef __EFFECTLIST_H
#define __EFFECTLIST_H

#include "ff.h"

typedef struct _effect_node {
	Effect *info;
	struct _effect_node *next;
}effect_node;


typedef struct _EffectList {
	effect_node *first;
	effect_node *last;
	int size;
}EffectList;




EffectList EffectList_Create();
void EffectList_Add(EffectList *l, Effect *x);
Bool EffectList_IsEmpty(const EffectList *l);
int EffectList_Size(const EffectList *l);
Effect* EffectList_Get(const EffectList *l, const int index); /* 1 <= index <= size*/
void EffectList_Print(const EffectList *l);



#endif
