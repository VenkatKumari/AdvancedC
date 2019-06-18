#include <stdio.h>
#include "francecity.h"

int main(){

    FILE * file;
    file = fopen("FRANCE.MAP", "r");
    if (!file) return 2;

    char city[50];
    char startcity[20], finishcity[20];
    int longitude, latitude;
    int returnValue;
    City * ct = malloc(sizeof(City));
    List * dp = newList(Eltcmp, CityDisplay);


    while(returnValue!=-1){
        returnValue = fscanf(file,"%s %d %d\n", city, &latitude, &longitude);
        City * alreadyVisited = lookupCity(dp, city);

        if (returnValue == 3){
            if(alreadyVisited == 0){
                ct =BuildCity(city, latitude, longitude);
                addList(dp, ct);
            }
            else
                {
                ct = alreadyVisited;
                ct->latitude = latitude;
                ct->longitude = longitude;
            }
        }

        else if (returnValue == 2){
            Neighbours * nb = malloc(sizeof(Neighbours));
            if (alreadyVisited==0){
                City * nbCity = BuildCity(city, -1, -1);
                addList(dp, nbCity);
                nb = CreateNeighbours(nbCity,latitude);
                addList(ct->neighbours, nb);
            }
            else
                {
                nb=CreateNeighbours(alreadyVisited, latitude);
                addList(ct->neighbours, nb);
            }
        }
    }

fclose(file);
    forEach(dp, CityDisplay);

    List * exposed = newList(citycmp, CityDisplay);
    List * bolted = newList(Eltcmp, CityDisplay);
    printf("\nEnter the starting city");
    gets(startcity);
    printf("\nEnter the destination city");
    gets(finishcity);

    City * actual = lookupCity(dp, startcity);
    City * reach = lookupCity(dp, finishcity);

    addList(exposed, actual);
    actual->startdist = 0;
    actual->finishdist=rennesToLyon(actual, reach);

    while(exposed->head){
        void * dp;
        remFromListAt(exposed, 1, &dp);
        addList(bolted, dp);
        City * ct = (City *) dp;

        if(ct == reach){
            printf("Reached:\n");
            while(ct){
                printf("%s %d %d \t",ct->cityname,ct->latitude,ct->longitude);
                ct = ct ->ctr;
            }
            return 0;
        }
        Node * nd = ct->neighbours->head;
        while(nd){

            City * achieve = ((Neighbours *)nd->val)->city;
            int cover =((Neighbours *)nd->val)->distance;
            int covered = ct->startdist + cover;

            if((isInList(exposed, achieve)!=0) ||
              ((isInList(bolted, achieve)!=0) &&
                    (covered > achieve->startdist)))
            {
                    nd = nd->next;
            }
            else{
                if(isInList(exposed, achieve)!=0){
                    remFromList(exposed, achieve);
                }
                if(isInList(bolted, achieve)!=0){
                    remFromList(bolted, achieve);
                }
                achieve->startdist = covered;
                achieve->ctr = ct;
                addList(exposed, achieve);
                nd = nd->next;
            }
        }
    }


printf("Invalid entry");
    return 0;
}

