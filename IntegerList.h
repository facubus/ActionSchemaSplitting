
#ifndef __INTEGERLIST_H
#define __INTEGERLIST_H


typedef long elem;

typedef struct node {
  elem info;
  struct node *next;
}node;

typedef struct _IntegerList {
  node *first;
  node *last;
  int size;
}IntegerList;




IntegerList IntegerList_Create();
void IntegerList_Add(IntegerList *l, elem x);
IntegerList IntegerList_Merge(const IntegerList *l1, const IntegerList *l2);
Bool IntegerList_IsEmpty(const IntegerList *l);
int IntegerList_Size(const IntegerList *l);
void IntegerList_Print(const IntegerList *l);
elem IntegerList_Get(const IntegerList *l, const int index); /* 1 <= index <= size*/
int IntegerList_GetIndex(const IntegerList *l, elem e); /*return -1 if e is not in l*/

void IntegerList_Set(IntegerList *l, int index, int value); /* 1 <= index <= size*/

Bool IntegerList_Contains(const IntegerList *l, const elem x);


int IntegerList_Maximum(const IntegerList *l, int k);
int IntegerList_Minimum(const IntegerList *l, int k);

void IntegerList_Sort(IntegerList *l); /*sort the list from highest to lowest*/

void IntegerList_Swap(IntegerList *l, int i, int j);

#endif
