
#ifndef __FLOATLIST_H
#define __FLOATLIST_H


typedef float f_elem;

typedef struct f_node {
	f_elem info;
	struct f_node *next;
}f_node;

typedef struct _FloatList {
	f_node *first;
	f_node *last;
	int size;
}FloatList;




FloatList FloatList_Create();
void FloatList_Add(FloatList *l, f_elem x);
Bool FloatList_IsEmpty(const FloatList *l);
int FloatList_Size(const FloatList *l);
void FloatList_Print(const FloatList *l);
f_elem FloatList_Get(const FloatList *l, const int index); /* 1 <= index <= size*/
void FloatList_Set(const FloatList *l, const int index, float value); /* 1 <= index <= size*/




int FloatList_Minimum(const FloatList *l, int k);
float FloatList_MinimumValue(const FloatList *l, int k);
int FloatList_Maximum(const FloatList *l, int k);
float FloatList_MaximumValue(const FloatList *l, int k);
void FloatList_Swap(FloatList *l, int i, int j);



f_elem FloatList_Average(const FloatList *l);



#endif
