#ifndef __List_H
#define __List_H
#include <stdlib.h>
#include "status.h"

//generic list structure
typedef struct Node {
    void *val;
    struct Node	*next;
} Node;

typedef int(*compFun)   (void* e1, void* e2);
typedef void(*prFun)   (void*);
typedef struct List {
    int nelts;
    Node * head;
    compFun comp;
    prFun pr;
} List;

List*	newList	(compFun,prFun);
void 	delList	(List* l);
status 	nthInList	(List* l,int n,void* res);
status 	addListAt	(List* l,int n,void* e);
status 	remFromListAt	(List* l,int n,void* res);
status 	remFromList	(List* l,void* e);
status 	displayList	(List* l);
void	forEach		(List* l,void(*f)(void*));
int	lengthList	(List* l);
status	addList	(List* l,void* e);
Node*	isInList	(List* l,void* e);

typedef struct City{
    //Details of the city
    char cityname[50];
    int latitude;
    int longitude;
   //list of neighbours to that city
    List * neighbours;
    //Creating a link between cities
    int startdist;
    int finishdist;
    struct City * ctr;
}City;

int DistBtwTwoCities(void *, void *);
void CityDisplay(void * );

typedef struct Neighbours{
    int distance;
    City * city;
}Neighbours;



void cpyString(char [], char []);
int currentToGo(City *);

//functions to compare between cities, neighbours and the distance between
int citycmp(void *, void *);
int Eltcmp(void *, void *);
int Neighbourcmp(void *, void *);
int rennesToLyon(City *, City *);


City * BuildCity(char*,int, int);
City * lookupCity(List * l, char[50]);
Neighbours * CreateNeighbours(City *,int);
void NeighboursDisplay(void *);

#endif



