#include <stdlib.h>
#include "routines.h"

double N;
int m;
int count;
int mm;
double count2;

int insert(Rtree tree, int ID, char* name, int parentID, int birthdate, char* birthlocation, int spouseID, int k, int* ofsID);
void lookup_sw(Rtree root_tr);
void load (Rtree root_tr);
void allchildren(Rtree root_tr);
void gchildren(Rtree root_tr);
void print(Rtree root_tr);
void ancestry(Rtree root_tr);
void gKchildren(Rtree root_tr);
void Kcousins(Rtree root_tr);
void avgoffspring(Rtree root_tr);
Rtree initialize();
