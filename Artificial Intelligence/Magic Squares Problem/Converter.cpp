#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

string convertToString(int integer) {
	
	char strArray [10];
	int n;
	n = sprintf(strArray, "%d", integer);
	string str = string(strArray);
	return str;
}

int convertToInt(string str) {
	
	char strArray[10];
	strcpy (strArray, str.c_str());
	int integer = atoi (strArray);
	return integer;
}

