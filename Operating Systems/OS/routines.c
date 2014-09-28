#include <stdio.h>
#include <stdlib.h>
#include "routines.h"

extern int m;
extern int count;
extern double N;
extern double count2;
extern int mm;

//dimiourgia tis rizas
nodetype root_creation()
{
	return NULL;
}

//dimiourgia tou dendrou
Rtree rooted_tree_cr()
{
	Rtree rooted_tree;
	rooted_tree = malloc( sizeof(struct tree));
	rooted_tree->root = root_creation();
	return rooted_tree;
}


//psaksimo sto dendro me anadromiko algorithmo
nodetype DFS ( nodetype node , int ID )
{
        int i;
        nodetype found;


        if ( node->ID == ID )                                                   //an vrethike o komvos, ton epistrefoume
                return node;
        if ( ( node->spouce != NULL )  &&  ( node->spouseID == ID ) ) //an enas komvos exei syzygo kai einai aytos pou psaxnoume
        {     
                return node;                                                    //epistrefoume ton arxiko komvo
        }
        
        for ( i = 0  ;  i < node->k  ;  i++ )                                   //alliws an de vrethike o komvos 
        {
             if( node->children[i] != NULL)                                     
                {
                        if ( ( found = DFS ( node->children[i], ID )) != NULL ) //kaloume tin synartisi anadromika,
                                                                                //gia ta paidia tou komvou pou eksetazoume 
                        return found;                                         
                }
         }
        return NULL;
}


//douleyei me ton idio tropo me tin DFS mono pou otan vrei oti o syzygos kapoiou komvou
//einai aytos pou psaxnoume dld, den anhkei sto dendro aytos pou psaxnoume , epistrefei
//ayton kai oxi ton syzygo tou, pou anhkei sto dendro
nodetype DFS_lookup ( nodetype node , int ID )
{
        int i;
        nodetype found;


        if ( node->ID == ID )
                return node;
        if ( ( node->spouce != NULL )  &&  ( node->spouseID == ID ) )
        {     
                return node->spouce;
        }
        for ( i = 0  ;  i < node->k  ;  i++ )
        {
             if( node->children[i] != NULL)
                {
                        if ( ( found = DFS_lookup ( node->children[i], ID )) != NULL )
                        return found;
                }
         }
        return NULL;
}

//psaxnei to dendro, opws kai i DFS , alla me vasi to onoma, me xrhsh ths synartisis strcmp
nodetype DFS_name ( nodetype node , char* name )
{
        int i;
        nodetype found;
      
      
        if ( strcmp(node->name,name) ==0 )
                return node;
        if ( ( node->spouce != NULL )  &&  (strcmp( node->spouce->name,name ) == 0))
        {     
                return node->spouce;
                }
        for ( i = 0  ;  i < node->k  ;  i++ )
        {
             if( node->children[i] != NULL)
                {
                        if ( ( found = DFS_name ( node->children[i], name )) != NULL )
                        return found;
                }
         }
        return NULL;
}

//psaxnei to dendro me vasi to onoma, opws i DFS_name, alla opws kai i DFS_lookup,
//otan vrei oti o komvos pou psaxnoume den anhkei amesa sto dendro, epistrefei 
//ton syzygo tou, pou anhkei sto dendro
nodetype DFS_name_ofs ( nodetype node , char* name )
{
        int i;
        nodetype found;
      
      
        if ( strcmp(node->name,name) ==0 )
        {  
                return node;
        }
        if ( ( node->spouce != NULL )  &&  (strcmp( node->spouce->name,name ) == 0))
        {     
                return node;
        }
        for ( i = 0  ;  i < node->k  ;  i++ )
        {
             if( node->children[i] != NULL)
                {
                        if ( ( found = DFS_name_ofs ( node->children[i], name )) != NULL )
                        return found;
                }
         }
        
        return NULL;
}

//ayti i synartisi pairnei ta orismata , dld ta stoixeia mias eggrafis pou eisagoume 
//me xrhsh tis synartisis insert ap eytheias
void insert_sw(Rtree root_tr)
{
         int i;
         int ID;
         int parentID;
         int DOB;
         int spouceID;
         int k;
         int kid;
         int *kidID;
         char* name;
         char* birthplace;
        
 
   
        name = malloc(20*sizeof(char));
        birthplace =  malloc (20*sizeof(char));
                         
        scanf(" %d %s %d %d %s %d %d", &ID, name , &parentID, &DOB, birthplace, &spouceID, &k);
				 
        kidID = malloc(k *sizeof(int));
 
        for(i = 0; i < k; i++)
        {
              scanf(" %d", &kid);
              kidID[i] = kid;
         }

       insert(root_tr,  ID, name, parentID,  DOB,  birthplace, spouceID,  k, kidID);
}

//anadromiki synartisi i opoia diatrexei olo to dendro, apo enan komvo k meta
//k ektypwnei olous tous apogonous tou
void  print_nodes(nodetype node, Rtree root_tr)
{
	int i;

	for(i = 0; i < node->k ; i++)                                      //gia ola ta paidia tou komvou apo ton opoio ksekiname
	{	
		if((node->parentID < 0) && (node != root_tr->root))            //an aytos o komvos den exei patera kai den einai i riza
		{
			if(node->spouce->children[i] != NULL)    //pairnoume ta paidia tou apo ton syzygo tou, poiu anhkei sto dendro
			{
			printf("%s ", node->spouce->children[i]->name);            //ektypwnoume ta stoixeia tous
			printf("%d ", node->spouce->children[i]->ID);
			printf("%d ", node->spouce->children[i]->birthdate);
			printf("%s\n", node->spouce->children[i]->birthlocation);
			print_nodes(node->spouce->children[i], root_tr);           //kai kaloume anadromika tin synartisi gia ola ta paidia
			}
		}
		else                                                            //alliws, an o komvos anhkei amesa sto dendro
		{
			 if(node->children[i] != NULL)                               //ektypwnoume ta stoixeia twn paidiwn tou
                         {
                         printf("%s ", node->children[i]->name);
                         printf("%d ", node->children[i]->ID);
                         printf("%d ", node->children[i]->birthdate);
                         printf("%s\n", node->children[i]->birthlocation);
                         print_nodes(node->children[i], root_tr);         //kai kaloume anadromika ti synartisi 
                         }
		}
	}

}

//xrhsimopoieitai apo ti synartisi ancestry, kai episkeptetai andromika olous tous
//komvous-progonous enos komvou
void ancestors(nodetype node, Rtree root_tr)
{
	
	if(node->parentID < 0)              //an o komvos apo ton opoio ksekiname den anhkei amesa sto dendro
	{	
		if(node->spouce->parent != NULL)                                //ksekiname apo ton syzygo tou
		{
			printf("%s\n", node->spouce->parent->name);                 //ektypwnoume to onoma tou patera 
			if(node->spouce->parent->spouce != NULL)                    
			printf("%s\n", node->spouce->parent->spouce->name);         //kai to onoma tou syzygou tou patera
			ancestors(node->spouce->parent, root_tr);                   //kaloume anadromika ti synartisi 
			
		}
	}
	else                                                                 //an o komvos anhkei sto dendro
	{
		printf("%s\n", node->parent->name);                             //ektypwnoume to onoma tou gonea
		if(node->parent->spouce != NULL)                                //kai to onoma tou syzygou tou gonea
		printf("%s\n", node->parent->spouce->name);
		ancestors(node->parent, root_tr);                               //kai kaloume anadromika ti synartisi gia ton gonea
		
	}
	


}

//ayti i synartisi xrhsimopoieitai apo ti synartisi gKchildren kai deixnei tin
//allilouxia metaksy pappou-eggonou
void concatenation(nodetype node , int id1)
{
	if(node->ID == id1)                                        //ksekinwntas apo to eggoni , deixnoume ti allilouxia               
	printf("%s\n", node->name);

	else
	{	
	 if(node->parentID<0)      //an o komvos den anhkei amesa sto dendro                                   
	 {
		if(node->spouce != NULL)    //kai exei syzygo, ksekiname apo ton syzygo
		{
		 printf("%s\n", node->spouce->name);   //ektypwnoume to onoma tou syzygou
		 if(node->spouce->parent != NULL)      //an aytos o komvos exei patera 
		 concatenation(node->spouce->parent , id1); //kaloume anadromika ti synartisi me ton patera ayton
		}
	 }
	 else                                   //alliws, an o komvos anhkei amesa sto dendro,dld exei patera
	 {
		printf("%s\n", node->name);         //ektypwnoume to onoma, kai 
		concatenation(node->parent, id1);   //kaloume anadromika ti synartisi me ton patera
	 }
	
	}	
}



//ayti i synartisi epistrefei ton patera enos komvou me ton opoio tin kaloume
nodetype cousin(nodetype node)
{
	int count;

	if(node->parentID < 0)
	{
	    if(node->spouce != NULL)
	 	{
	  	 if(node->spouce->parent != NULL)
	   	 return node->spouce->parent;

	 	}
	}
	else
 	{
		if(node ->parent != NULL)
		return node->parent;


	}
}

//episkeptetai anadromika tous komvous tou dendrou kai metraei, posa paidia
//exei kathe komvos kathws kai posa apo ayta ta paidia exooun k ayta paidia
void average(nodetype node)
{
	int i;
		
	count2 = count2 + node->k;                         //metritis pou ayksanetai kata ton arithmo twn
                                                       //paidiwn pou exei o komvos pou episkeptomaste
	for(i = 0 ; i < node->k ; i++)                     //gia ola ta paidia tou komvou
	{	 
		if(node->children[i] != NULL)
		{
		  if(node->children[i]->k != 0)                //koitame poia paidia exoun k ayta paidia
		  N =  N + 1;                                  //kai gia kathe paidi pou exei paidia ayksanoume ton metriti kata 1

		  average(node->children[i]);                  //kaloume anadromika ti synartisi gia kathe paidi
		}

	}

}


