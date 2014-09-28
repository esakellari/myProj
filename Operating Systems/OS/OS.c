#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "OS.h"


extern int m;
extern int count;
extern double count2;
extern double N;
extern int mm;

//arxikopoihsh tou dendrou
Rtree initialize()
{
      return  rooted_tree_cr();
}

//synartisi eisagwgis eggrafwn sto dendro
int insert(Rtree tree, int ID, char* name, int parentID, int birthdate, char* birthlocation, int spouseID, int k, int* ofsID)
 {
	nodetype parent;           
	nodetype newnode;
	nodetype spouce;
	int i;
	int ii;
	
	//desmeyetai xwros gia to neo komvo tou dendrou kai pernwntai oi parametroi tou
     newnode = malloc( sizeof ( struct node));
	
	 newnode->name = malloc( (strlen(name) + 1)* sizeof (char));
     strcpy( newnode->name, name);
 
     newnode->birthlocation = malloc ((strlen( birthlocation)+1) * sizeof(char));
     strcpy(newnode->birthlocation, birthlocation);
     newnode->ofsID = malloc ( k * sizeof(int));
     newnode->children = malloc (k * sizeof(nodetype));
         
         
     for(ii=0;ii<k;ii++)
	 newnode->children[ii]=NULL;
         
     newnode-> k = k;
     newnode->birthdate = birthdate;
     newnode -> ID = ID;
     newnode->parentID = parentID;
     newnode->spouseID = spouseID;
         
     for(i =0; i < k ; i++)
     newnode->ofsID[i] = ofsID[i];
         
     newnode->spouce = NULL;
     newnode->parent=NULL;	
         
         
     //an to dendro den exei akoma riza    
	if ( tree->root ==  NULL)
	{	
		tree->root = newnode;                                    //o neos komvos einai i riza
		newnode -> parent = NULL;                                //kai den exei patera
	
 		mm++;                                                    //ayksanoume ton metrhth eisagwgis eggrafwn
	}

    //an o neos komvos exei patera
	else  if( parentID >= 0 )
	{ 
	    parent = DFS(tree->root, parentID);                      //psaxnoume ton patera sto dendro

		
		if(parent != NULL)
 		{      
             mm++;                                              //ayksanoume ton metrhth eisagwgis eggrafwn kai
			newnode->parent = parent;                           //o deiktis tou patera tou neou komvou deixnei ston patera p vrhkame
	            
             //psaxnoume na doume se poia thesi tou pinaka paidiwn tou patera paei to neo paidi
			for ( i = 0; i < parent->k; i++)
			{	
				if( parent->ofsID[i] == ID)                        //an to vrhkame, stamatame
				break;
                		if(i==parent->k-1)                         //an elegksame ola ta stoixeia tou pinaka
                		{
                  			if(parent->ofsID[i]==ID)               //an to paidi einai to teleytaio stoixeio tou pinaka , ola ok
                  			break;                                 
                  			else                                   //alliws, de vrhkame to paidi
                  			i=-1;
                		}
			}
		    
			if(i!= -1)
  	        {   
	 	        parent->children[i] = newnode;
         		newnode->parent = parent;		
          	}
          	
          	//an to id tou paidiou den yparxei ston pinaka twn id twn paidiwn tou patera
 		   else if( i == -1)
    		{
                //desmeyoume xwro ston pinaka deiktwn twn paidiwn toy patera 
   			  parent->children = realloc(parent->children, (parent->k +1)*sizeof(nodetype));
   			  //desmeyoume k allo xwro ston pinaka twn id twn paidiwn tou patera
              parent->ofsID = realloc(parent->ofsID, (parent->k +1)*sizeof(int));
              //enhmerwnoume to stoixeio tou pinaka me ton deikti sto neo komvo
   			  parent->children[parent->k] = newnode;
   			  //vazoume to id tou neou komvou sti nea thesi tou pinaka
   			  parent->ofsID[parent->k] =  newnode->ID;
   			  //enhmerwnoume to deikti tou patera tou neou komvou
   			  newnode->parent = parent;
   			  parent->k = parent->k+1;
   			  printf("record inserted!\n");
			}
  	     }
		else
		{
         return 0;
         }
	}	
	
	//an den exei patera o neos komvos
	else  if (parentID < 0)
	{	
		  spouce = DFS( tree->root, spouseID);                        //psaxnoume to syzygo
	  
		  if( spouce != NULL)                                           //an yparxei o syzygos
		  {
 		             newnode -> spouce = spouce;                        //enhmerwnoume tous deiktes tou neou komvou
		          	 spouce -> spouce = newnode;                        //kai tou syzygou na deixnoun o enas ston allo
				
			/*	if(spouce->k > newnode->k)
				{	
					newnode->children = realloc(newnode->children, (spouce->k)*sizeof(nodetype));
					newnode->ofsID = realloc(newnode->ofsID, (spouce->k)*sizeof(int));
					newnode->k = spouce->k;		
				}
				
				for(i = 0 ; i < newnode->k; i++)
				{
					if(spouce->children[i] != NULL)
					newnode->children[i] = spouce->children[i];
				}
			 */	 
				mm++;                                                       
		  }
         	 
		else
	    { 
		  newnode -> spouce = NULL;
 	      return 0;
       	}
	}

	 
}


//synartisi pou psaxnei mia eggrafi sto dendro kai emfanizei ta stoixeia tis
void lookup_sw(Rtree root_tr)
{	
	nodetype node;
	int ID;
	int i;
	int arg;
	char input[30];
	
	scanf(" %s", input);
	
    if(isdigit(input[0]) != 0 )                                 //an dothike onoma gia na psaxoume tin eggrafi
    {
        arg = atoi(input);
        node = DFS_lookup(root_tr->root, arg);                  //psaxnoume tin eggrafi
    }
	
    
	else
    node = DFS_name(root_tr->root, input);                      //alliws psaxnoume me vasi to id 
	
	//molis  vroume tin eggrafi, ektypwnoume ta stoixeia tis
    if( node != NULL)
    {
    printf("name = %s\n", node->name);
    printf("ID = %d\n", node->ID);
    printf("parentid = %d\n", node->parentID);
    printf("birthlocation = %s\n", node->birthlocation);
    printf("date of birth = %d\n", node->birthdate);
    }



} 


//fortwnei eggrafes apo ena arxeio 
void load (Rtree root_tr)
{		
	FILE *fp;
	char file[15];
	int c;
	int ID;
	int parentID;
	int spouceID;
	int birthdate;
	int k, i;
	int DOB;
	int kid;
	int *kidID;
	char* name;
	char* birthplace;

	m = 0;

	name = malloc(15*sizeof(char));
    birthplace =  malloc (15*sizeof(char));


	
	scanf("%s", file);

	fp = fopen(file, "r");

    //diavazoume ta stoixeia tis kathe eggrafis
	while( !feof(fp))
	{	
	
          fscanf(fp , "%d ", &ID);
		  fscanf(fp , "%s ", name);
		  fscanf(fp , "%d ", &parentID);
		  fscanf(fp , "%d ", &DOB);
		  fscanf(fp , "%s ", birthplace);
		  fscanf(fp , "%d ", &spouceID);
		  fscanf(fp , "%d ", &k);
					
         kidID = malloc(k *sizeof(int));
                
         for(i = 0; i < k; i++)
         {
           fscanf( fp, "%d ", &kid);
           kidID[i] = kid;
         }
                    //kaloume tin synartisi insert gia eisagwgi mesa sto dendro
                insert(root_tr,  ID, name, parentID,  DOB,  birthplace, spouceID,  k, kidID);
                 
        }
	printf("%d records inserted \n", mm);
	fclose(fp);
 	free(name);
	free(birthplace);
	free(kidID);

}


//synartisi pou ektypwnei ola ta paidia enos komvou
void allchildren(Rtree root_tr)
{
    nodetype node;
	int ID;
	int arg;
	char input[30];
	int i;
	
	scanf(" %s", input);
	
	if(isdigit(input[0]) != 0 )                                 //an dothike onoma gia na psaxoume tin eggrafi
    {
        arg = atoi(input);
        node = DFS(root_tr->root, arg);
    }

    else 
   	node = DFS_name_ofs(root_tr->root, input);                  //alliws psaxnoume me vash to id

    
    if( node != NULL)
    {   
        printf("children of this node are:\n");
        for(i = 0; i < node->k; i++)                            //ektypwnoume ola ta paidia aytou tou komvou
        {
        if( (node->children[i]) != NULL)
        printf("%s\n", node->children[i]->name);
        }
     }
     
}

//synartisi p ektypwnei ta eggonia enos komvou
void gchildren(Rtree root_tr)
{
     nodetype node;
	int ID;
	int arg;
	char input[30];
	int i,m;
	
	scanf(" %s", input);

	if(isdigit(input[0]) != 0 )                                 //an dothike onoma gia na psaxoume tin eggrafi
    {
        arg = atoi(input);
         node = DFS(root_tr->root, arg);
    }
  
	else 
    node = DFS_name_ofs(root_tr->root, input);                  //alliws psaxnoume me vasi to id
   
    
    if( node != NULL)
    {   
        printf("grandchildren of this node are :\n");           //ektypwnontai ta onomata twn eggonwn 
        for ( i =  0; i < node->k; i++)
            {   if( node->children[i] != NULL)
                {
                 for(m = 0 ; m < node->children[i]->k; m++)
                   {
                   if(node->children[i]->children[m] != NULL)
                   printf("%s\n", node->children[i]->children[m]->name);
                   }
                 }
            }
    }
     
     
}



void print(Rtree root_tr)
{
	 nodetype node;
     int ID;
     int arg;
     char input[30];
     int i,m;
 
        
     scanf(" %s", input);
	
    if(isdigit(input[0]) != 0 )                                 //an dothike onoma gia na psaxoume tin eggrafi
    {
        arg = atoi(input);
        node = DFS(root_tr->root, arg);
    }
         

     else 
     node = DFS_name(root_tr->root, input);                     //psaxnoume me vasi to id
	 

    if( node != NULL)
     {
	print_nodes(node, root_tr);                                 //an yparxei o komvos kaleitai h anadromiki synartisi gia ektypwsi
     }

}


//ektypwnontai oi progonoi enos komvou

void ancestry(Rtree root_tr)
{
	int id;
	nodetype node;

	scanf("%d", &id);
	node = DFS_lookup(root_tr->root, id);                       //vriskoume ton komvo pou theloume
	
	if( node != NULL)
	ancestors(node, root_tr);	                                //kaleitai i anadromiki synartisi gia ektypwsi twn  progonwn
	
}

//elegxei an dyo atoma pou dothikan exoun sxesi pappou-eggoniou
void gKchildren(Rtree root_tr)
{
	int id1, id2;
	nodetype node;
	nodetype gchild;

	scanf("%d %d", &id1, &id2);
	node = DFS_lookup(root_tr->root, id1);                                    //psaxnoume prwta ton "pappou"
	
	if( node != NULL)                                                         //an yparxei o pappous
	{	
		if((node->parentID < 0) && ( node != root_tr->root))                  //an aytos o komvos den exei patera kai den einai i riza
		{
			if((gchild = DFS_lookup(node->spouce, id2)) != NULL)              //psaxnoume me afethria ton syzygo aytou tou komvou, to eggnoni
			{
				printf("%d is grandchild of %d with concatenation:\n", id2, id1);       
				concatenation(gchild, id1);                                       //ektypwnoume tin allilouxia metaksy pappou-eggonou
			}
			else printf("%d is not grandchild of %d\n", id2, id1);	
		}
		else                                                        //an o pappous einai komvos tou dendrou
		{
			if((gchild = DFS_lookup(node, id2)) != NULL)           //psaxnoume to eggoni me afethria ayton ton komvo
			{
		  		printf("%d is grandchild of %d with concatenation:\n", id2 , id1);     //molis to vroume 
		  		concatenation(gchild, id1);                                            //ektypwnoume tin allilouxia
			}
			else printf("%d is not grandchild of %d\n", id2, id1);
		}
	}
		
	
}


//elegxoume an dyo atoma p dwsane einai ksaderfia kai an nai, ti vathmou
void Kcousins(Rtree root_tr)
{
	int id1, id2;
	nodetype node1, node2;
	
	count  = 0;
	
	scanf("%d %d", &id1, &id2);
	
	node1 = DFS_lookup(root_tr->root, id1);                                    //psaxnoume tous dyo komvous 
	node2 = DFS_lookup(root_tr->root, id2);
		
	do
	{
		node1= cousin(node1);                                                  //pairnoume tous goneis twn dyo atomwn piu dothikan
		node2= cousin(node2);                                                  //mexri na vrethoume se enan koino progono
		count++;
	        if(node1->ID == node2->ID )                                         //moilis vroume ton koino progono stamatame
		break;           
		if( (node1 == root_tr->root) || (node2 == root_tr->root))               //an ftasame sti riza alla oxi taytoxrona kai apo tous dyo
		{                                                                       //shmainei oti ta dyo atoma den einai skaderfia
			printf("%d is not cousin to %d\n", id1, id2);
			break;

		}
	}while(node1->ID != node2->ID);


	if(node1->ID == node2->ID )                                                 //molis vrethoume se koino progono
	{
	  if( count == 1)
	  printf("%d is brother to %d\n", id1, id2);                                //an exoume ftasei se progono me tin prwti epanalipsi, einai aderfia
	  else 
	  printf("%d is cousin to %d with distance %d\n", id1, id2, count-1);       //alliws ektypwnoume ton vathmo syggeneias


	}
	else 
	printf("%d is not cousin to %d\n", id1 , id2);
	count  = 0;
}

//ektypwnetai o mesos oros apogonwn 
void avgoffspring(Rtree root_tr)
{
 	nodetype node;
        int ID;
        int arg;
        char input[30];
        int i,m;
        double avg;

	N = 0;
	count2  = 0;
 
        scanf(" %s", input);
 
    if(isdigit(input[0]) != 0 )                                 //an dothike onoma gia na psaxoume tin eggrafi
    {
        arg = atoi(input);
        node = DFS_lookup(root_tr->root, arg);
    }
          
 
      else 
      node = DFS_name(root_tr->root, input);                      //alliws psaxnoume me vasi to id
         
	
	if(node != NULL)                                              //an yparxei o komvos
	{	if( (node->parentID < 0 ) && (node != root_tr->root))     //kai den einai i riza
		{
			if(node->spouce!= NULL)                               //an epistrafike komvos pou den einai sto dendro
			node = node->spouce;                                  //ksekiname apo ton syzygo aytou tou komvou
		}
		average(node);                                            //kaleitai i anadromiki synartisi
	}

	if( N != 0)
	{
	    avg  = count2/N;
        printf("average of offsrpings is %0.3f\n",avg );        
    }


}













