/*        сайеккаяг екисабет 1115200600152      */

#include <iostream>
#include <list>
#include <stdexcept>
#include <fstream>
#include <string>
#include <stdio.h>

#include "../framework/GraphSearch.h"
#include "../framework/TreeSearch.h"
#include "../informed/AstarSearch.h"

#include "BlocksState.h"
#include "BlocksGoalTest.h"
#include "BlocksSuccessor.h"
#include "blocksToBePlacedHeuristic.h"

using namespace std;

int main(void)
{
   string init, goal;
   vector<int> Init, Ggoal;
   
   ifstream infile;
   
   //Aanoigw to arxeio gia na diavasw to input
   infile.open("input23.txt");
    
    //arxiki katastasi
   getline(infile, init);
   //teliki katastasi
   getline(infile, goal);
    
   cout <<"Initial State = "<< init << " \nGoal State = " << goal << endl;
  
   string str;
   
   //afiarw tis teleies kai krataw tous arithmous kai tous vazw sto str
   //kai sti synexeia sto vector 
   for(int i = 0; i < init.size(); i++)
 	{     
          if(init[i] != '.' && init[i] != 13)
          str.push_back(init[i]);
           
           else
           {
               Init.push_back(atoi(str.c_str()));
               str.clear();
           }
    }
 


   for( int i=0; i <goal.size(); i++)
   {
        if(goal[i] != '.' && goal[i] != 13)
        str.push_back(goal[i]);
           
        else
        {
             Ggoal.push_back(atoi(str.c_str()));
             str.clear();
         }
   }    
     
 cout << "SIZE = " << Init.size()<< endl;


   BlocksState initial(Init,Ggoal);
   initial.SetCurrState(initial.GetInitState());
    
   BlocksSuccessor succFunc = BlocksSuccessor();
   BlocksGoalTest fgoal;

   GraphSearch gsearch = GraphSearch();
   TreeSearch treeSearch = TreeSearch();
   AstarEvaluationFunction f = AstarEvaluationFunction();
   DefaultStepCostFunction stepCostFunc = DefaultStepCostFunction();
   blocksToBePlacedHeuristic heur = blocksToBePlacedHeuristic();
   Problem problem = Problem(initial, succFunc, fgoal, stepCostFunc, heur);
   AstarSearch astar(gsearch, f);
	
	try {
		Node* mgoal = astar.search(f, problem);
		if (mgoal == NULL) {
			cout << "Failed to find solution." << endl;
		} else {
			cout << "Solution found!!!!!." << endl;
			mgoal->printNode();
			mgoal->getState().print();
			mgoal->printActionPathFromRoot();
			cout << endl;
			gsearch.printMetrics();
			
		}
		
	} catch (exception &e) {
		cout << "Exception: " << e.what() << endl;
	}
	
	system("PAUSE");
	return 0;
   
    
}
