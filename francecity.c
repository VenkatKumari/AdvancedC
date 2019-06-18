#include <stdio.h>
#include "francecity.h"
#include <string.h>
#include <math.h>

static Node* available = 0;

List * newList (compFun comp, prFun pr) {
  List * l;
  l = (List*) malloc(sizeof(List));
  if (! l) return 0;
  l->nelts	= 0;
  l->head	= 0;
  l->comp 	= comp;
  l->pr     = pr;
  return l;
}

City * BuildCity(char* cityname, int latitude, int longitude){
    City * ct = malloc(sizeof(City));
    cpyString(ct->cityname,cityname);

    ct->latitude = latitude;
    ct->longitude = longitude;
    ct->startdist = -HUGE_VAL;
    ct->neighbours = newList(Neighbourcmp, CityDisplay);
    ct->neighbours->comp=Neighbourcmp;
    //c->link = 0;
    return ct;
}

/* destroy the list by deallocating used memory */
void delList (List* l) {
  Node * tmp = l->head;
  while (tmp) {
	l->head = tmp->next;
    tmp->next = available;
    available = tmp;
    tmp = l->head;
  }

  /* tmp == 0 : list is empty */

  l->head = 0;
  l->nelts = 0;
  free(l);
}

City * lookupCity(List * l,char cityname[100]){
    if(!l->head) return NULL;
    Node * nd = l->head;
    while(nd){

        if(strcmp(((City *)(nd->val))->cityname, cityname) == 0){
            return nd->val;
        }
        nd=nd->next;
    }
    return 0;
}


/* return  the Nth element of the list if exists */
status nthInList (List* l, int pos, void* res) {
  Node * tmp = l->head;
  if (pos <= 0 || pos > l->nelts)
    return ERRINDEX;
  while (pos-- > 1) tmp = tmp->next;
  *(void**)res = tmp->val;
  return OK;
}


void CityDisplay(void * disp){
    City * dp = disp;
    printf("\n %s  %d  %d  %d  \t", dp->cityname,dp->latitude,dp->longitude,dp->neighbours->nelts);
    forEach(dp->neighbours, NeighboursDisplay);
}

status addListAt (List* l, int pos, void* elt) {
    if (pos <= 0 || pos > l->nelts+1)
        return ERRINDEX;
    else {

        /* get a new Node and increment length */
        Node * toAdd = available;
        if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
        else available = available->next;
        if (!toAdd) return ERRALLOC;
        l->nelts++;
        toAdd->val = elt;

        /* if pos is 1, must change list head */
        if (pos == 1) {
            toAdd->next = l->head;
            l->head = toAdd;
        }

        /* otherwise we need a temporary pointer */
        else {
            Node * tmp = l->head;

            /* tmp points to the predecessor */
            while (pos-- > 2) tmp = tmp->next;

            /* actual insertion */
            toAdd->next = tmp->next;
            tmp->next = toAdd;
        }
        return OK;
    }
}

int Neighbourcmp(void * neighboura, void * neighbourb){
    int calculatepath = ((Neighbours*)neighboura)->distance + (~((Neighbours*)neighbourb)->distance + 1);
    return calculatepath;
}

status remFromListAt	(List* l,int pos, void* res) {
  Node *toRem = l->head;

  if (pos <= 0 || pos > l->nelts) return ERRINDEX;

  /* particular case: must remove list head */

  if (pos == 1)
    l->head = toRem->next;

  /* otherwise point to predecessor to change links */
  else {
    Node * prec = toRem;
    while (pos-- > 2) prec = prec->next;
    toRem = prec->next;
    prec->next = toRem->next;
  }

  *(void**)res = toRem->val;
  toRem->next = available;
  available = toRem;
  l->nelts--;
  return OK;
}

int Eltcmp (void * statea, void * stateb) {

    if (!statea && !stateb) return 0;
    if (statea && !stateb) return 1;
    if (!statea && stateb) return -1;

    while (statea && stateb && statea == stateb) statea++, stateb++;

    if (!statea && !stateb) return 0;
    else if (statea > stateb) return 1;
    else return -1;
}

/* remove given element from given list */
status remFromList (List* l, void* elt) {
  Node * prec = l->head, *toRem = 0;

  if (l->comp == 0) return ERRUNABLE;
  if (l->nelts == 0) return ERRABSENT;
  if (!(l->comp)(elt,prec->val)) return remFromListAt(l,1,&(prec->val));

  /* points to the predecessor */

  while (prec->next &&  (l->comp)(elt,prec->next->val))
    {
      prec = prec->next;
    }

  /* here
   *  - either we point to the last element
   *  - or next one is the one to remove!
   */

  if (! prec->next) return ERRABSENT;

  toRem = prec->next;
  prec->next = toRem->next;
  toRem->next = available;
  available = toRem;
  l->nelts--;

  return OK;
}


void NeighboursDisplay(void * disp){
    Neighbours * nbs = disp;
    printf("%s%d\n",nbs->city->cityname,nbs->distance);
}

/* display list elements as "[ e1 ... eN ]" */
status displayList (List * l ) {
  Node * tmp = l->head;
  if (l->pr == 0) return ERRUNABLE;
  printf("[ ");
  while (tmp) {
    (l->pr)(tmp->val);
    putchar(' ');
    tmp = tmp->next;
  }
  putchar(']');
  return OK;
}

int DistBtwTwoCities(void * citya, void * cityb){


    int cp = ((City *)citya)->startdist + (~((City *)citya)->startdist + 1);
    return cp;
}

/* sequencially call given function with each element of given list */
void forEach (List* l, void(*f)(void*)) {
  Node * tmp = l->head;
  while (tmp) {
    (*f)(tmp->val);
    tmp = tmp->next;
  }
}

int lengthList(List* l) {
	return l->nelts;
}


int rennesToLyon(City * ct,City * finish){

    float distance;
    distance = sqrt((ct->latitude - finish->latitude) * (ct->latitude - finish->latitude) + (ct->longitude - finish->longitude) * (ct->longitude - finish->longitude));
    return distance;
}

static Node* previous(List* l, void* e) {
  Node * prec = l->head;
  if (l->nelts == 0) return 0;
  if (!(l->comp)(e,prec->val)) return (Node*)1;

  /* prec must address element prior to the one to remove */
  while (prec->next && (l->comp)(e,prec->next->val))
    prec = prec->next;

  /* here,
   *  - either we address nothing (no such element)
   *  - or we address the element prior to the one we're looking for
   */
  return prec;
}

int currentToGo(City * ct){

    while(ct->startdist) {
        int t = (ct->startdist & ct->finishdist) <<1;
        ct->finishdist ^= ct->startdist;
        ct->startdist = t;
    }
    return ct->finishdist;
}

/* add given element to given list according to compFun function */
status addList (List* l, void* e) {
  Node * prec = l->head, *toAdd;

  if (l->comp == 0) return ERRUNABLE;

  /* add to the head if list is empty, if no comparison function is given
   * or if e < head element
   */
  if (l->nelts == 0 || (l->comp)(e,l->head->val)<0)
    return addListAt(l,1,e);

  /* otherwise, get predecessor and link new element just after it */
  while (prec && prec->next && (l->comp)(prec->next->val,e)<0)
	  prec = prec->next;

  toAdd = available;
  if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
  else available = available->next;
  if (!toAdd) return ERRALLOC;
  toAdd->next = prec->next;
  toAdd->val = e;
  prec->next = toAdd;
  l->nelts++;
  return OK;
}


/* test whether the list contains given element */
Node* isInList (List* l, void* e) {
  Node * prec = previous(l,e);
  if (!prec) return 0;
  if (prec == (Node*) 1) return (Node*)1;
  if (prec->next == 0) return 0;
  if (!(l->comp)(e,prec->next->val))
    return prec;
  return 0;
}

int citycmp(void * city1, void * city2){
    City * c1 = city1;
    City * c2=  city2;
    return (currentToGo(c1)+currentToGo(c2))/2;
}

void cpyString(char d[], char s[]) {
   int c = 0;

   while (s[c] != '\0') {
      d[c] = s[c];
      c++;
   }
   d[c] = '\0';
}

Neighbours * CreateNeighbours(City * city, int distance){

    Neighbours * nb = malloc(sizeof(Neighbours));
    nb->city = city;
    nb->distance = distance;
    return nb;
}

