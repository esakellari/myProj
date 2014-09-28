/*        сайеккаяг екисабет 1115200600152      */

#include "BlocksSuccessor.h"
#include "BlocksState.h"

BlocksSuccessor::BlocksSuccessor()
{
}

void BlocksSuccessor::getSuccessors(const State &state, list<Successor*> *succ)
{
     vector<int> curr,init, goal,free,next;
     
     BlocksState &initial = (BlocksState &) state;
     
     curr = initial.GetCurrState();
     init = initial.GetInitState();
     goal = initial.GetGoalState();


     for(int i = 0; i < curr.size(); i++)
     {
             //an o kyvos einai se swsti thesi de to kounaw katholou
             if(initial.BlockIsCorr(curr, goal, i) == true)
             continue;
             
             //an o kyvos i den exei kanena apo panw tou mporei na metakinithei
             if(initial.IsFreeAbove(curr, (i+1)) == true)
              {
                 BlocksState *child = new BlocksState(init, goal);  
                   
              //an den einai panw sto trapezi ton topothetw sto trapezi
              //an einai panw se kyvo-koryfi
              if(MoveTopCube(curr, i,initial, child)!= ON_TABLE)
                {
                   
         	       ostringstream act;
			       act << "Moved Block number  " << (i+1)<<" to table" ;
			       string action = act.str();
			       
			       Successor *successor = new Successor(action, *child);
			       succ->push_back(successor);
			       free = initial.freeBlocks(curr);
			    
			       //ton topothetw kai panw stous eleytherous kyvous
		           for(int ii = 0; ii < free.size(); ii++)
  	               {
  	                 if(free[ii] == (i+1))
				     continue;
                    	
                     BlocksState *child = new BlocksState(init, goal);
                     next.clear();
                                
                     for(int j = 0; j < curr.size(); j++)
                     next.push_back(curr[j]);
                         
                     next.at(i) = free[ii];
                                                   
                     child->SetCurrState(next);
      		         ostringstream act;
			         act << "Moved Block number  " << (i+1) ;
			         string action = act.str();
			         Successor *successor = new Successor(action, *child);
		             succ->push_back(successor);  
     	           }
                 }
                  
        else 
        {
             free = initial.freeBlocks(curr);

              //ton topothetw kai panw stous eleytherous kyvous
		      for(int ii = 0; ii < free.size(); ii++)
  	          {
                    next.clear();
                   
                    if(free[ii] == (i+1))
  		    	    continue;

                    BlocksState *child = new BlocksState(init, goal);
                                
                    for(int j = 0; j < curr.size(); j++)
       	             	next.push_back(curr[j]);
                             	
                     next.at(i) = free[ii];
   
                     child->SetCurrState(next);
      		         ostringstream act;
			       	 act << "Moved Block number  " << (i+1) ;
			         string action = act.str();
			         Successor *successor = new Successor(action, *child);		 
				     BlocksState &st2 = (BlocksState &)(successor->getState());
			 	     st2.print();
			         succ->push_back(successor);  
                }
          }
                  
  	}
   }


}

int BlocksSuccessor::MoveTopCube(vector<int> curr,int pos, BlocksState &parent, BlocksState *child)
{
     vector<int> next;
         
     for(int i = 0; i < curr.size(); i++)
     next.push_back(curr[i]); 
            
      //an den einai panw sto trapezi to vazw panw sto trapezi 
     if(parent.IsOnTable(curr, pos)==false)
     {
         next.at(pos) = 0;
         child->SetCurrState(next);
         return 0;
     }
     
     else 	return ON_TABLE; 

}

BlocksSuccessor::~BlocksSuccessor()
{
}
