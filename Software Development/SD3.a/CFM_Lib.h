#ifndef _CFM_LIB_H
#define _CFM_LIB_H

#include "UT_DM_Defines.h"


int CFM_Create();
int CFM_AddRel(relDesc rel, attrDesc* attrs);
int CFM_DeleteRel(char* relName);
int CFM_IndexAttribute(char* relName, char* attrName, int indexNo);
int CFM_Lookup(char *relName,relDesc *rel, attrDesc **attrs);

#endif

