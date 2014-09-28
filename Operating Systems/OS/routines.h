#ifndef _ROUT_
#define _ROUT_

extern int m;
extern int count;
extern double N;
extern int mm;
extern double count2;

//orismos typou dendrou
typedef struct tree *Rtree;

//orismos typou komvou
typedef struct node *nodetype;

typedef struct node
	{
		int ID;
		char* name;
		int parentID;
		int birthdate;
		char* birthlocation;
		int spouseID;
		int k;
		int* ofsID;
		nodetype parent;
		nodetype* children;
		nodetype spouce;
	
	}node;


typedef struct tree
	{
		nodetype root;
	}tree;


//dhlwseis synartisewn
void insert_sw(Rtree root_tr);
nodetype DFS ( nodetype node , int ID );
nodetype DFS_lookup ( nodetype node , int ID );
nodetype DFS_name ( nodetype node , char* name );
nodetype DFS_name_ofs ( nodetype node , char* name );
nodetype cousin(nodetype node);


void  print_nodes(nodetype node, Rtree root_tr);
void ancestors(nodetype node, Rtree root_tr);
void concatenation(nodetype node , int id1);
void average(nodetype node);


Rtree rooted_tree_cr();
nodetype root_creation();






#endif
