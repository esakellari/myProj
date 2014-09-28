/*        сайеккаяг екисабет 1115200600152      */

#include "BlocksState.h"
#include <unistd.h>

//constructor
BlocksState::BlocksState(vector<int> Init,vector<int> Goal)
{
    int i;
     
    for (i=0;i<Init.size();i++)
     	init_state.push_back(Init[i]);
	                       
    for (i=0;i<Goal.size();i++)
     	goal_state.push_back(Goal[i]);
	     
    
            
          	Setweightgoal(goal_state, weightgoal);
   	        SetUndergoal(goal_state); 
       
       
}
//-------------------------------------------accessors-------------------------//
vector<int> BlocksState::GetGoalState() const
{
            return goal_state;
}

vector<int> BlocksState::GetInitState() const
{
            return init_state;
}

vector<int> BlocksState::GetCurrState() const
{
            return curr_state;
}

vector<int> BlocksState::GetUnderneath() const
{
            return underneath;
}

vector<int> BlocksState::GetUndergoal() const
{
            return undergoal;
}


vector<int> BlocksState::Getweight() const
{
            return weightcur;
}

vector<int> BlocksState::Getweightgoal() const
{
            return weightgoal;
}

int BlocksState::GetH()
{
    return height;
}



//--------------------------------------------mutators-------------------------//
void BlocksState::SetCurrState(vector<int> curr)
{	
     int i;
     vector<int> goal;
     
     for (i=0;i<curr.size();i++)
    curr_state.push_back(curr[i]);
    
   // SetUnderneath();
	
	Setweightcur(curr_state, weightcur);
    SetUnderneath(curr_state);
   

}

void BlocksState::SetGoalState(vector<int> goal)
{

     for (int i=0;i<goal.size();i++)
     	goal_state.push_back(goal[i]);
	  
}

void BlocksState::SetInitState(vector<int> init)
{
          
     for (int i=0;i<init.size();i++)
     	init_state.push_back(init[i]);
   
}

void BlocksState::SetUnderneath(vector<int> stat)
{
     for(int i = 0; i < stat.size(); i++)
     {
             if(stat[i] == 0)
             underneath.push_back(-1);
             
             else
             underneath.push_back(curr_state[i+1]);
     }
}

void BlocksState::SetUndergoal(vector<int> goal)
{
      for(int i = 0; i < goal.size(); i++)
     {
             if(goal[i] == 0)
             undergoal.push_back(-1);
             
             else
             undergoal.push_back(goal_state[i+1]);
     }
}



void BlocksState::SetH(int i)
{
     height = i;
}

void BlocksState::SetW(int i)
{
     weight = i;
}


void BlocksState::incrweight()
{
     weight++;
}

/*synartisi pou thetei times sto vector weightcur, to opoio krataei ta vari gia 
 *kathe kyvo stin parousa katastasi .
 */
void BlocksState::Setweightcur(vector<int> stat, vector<int> Weight)
{
    
     weightcur.clear();
         
     for(int i = 0; i < stat.size(); i++)
     { 
             SetW(0);
             //an den exei allo apo panw einai varos 0
             if(IsFreeAbove( stat,i+1) == true)
                weightcur.push_back(0);
                                    
             else
             {
                 //kaleitai i anadromiki synartisi gia ton ypologismo tou varous
                 calcWeight(stat,i+1);
                 weightcur.push_back(weight);
             }
     }
     
}

/*synartisi pou thetei times sto vector weightgoal, to opoio krataei ta vari gia 
 *kathe kyvo stin katastasi stoxou.
 */
void BlocksState::Setweightgoal(vector<int> stat, vector<int> Weight)
{
    
     weightgoal.clear();
         
     for(int i = 0; i < stat.size(); i++)
     { 
             SetW(0);
             
             //an den exei kanena apo panw tou, exei varos 0
             if(IsFreeAbove( stat,i+1) == true)
                weightgoal.push_back(0);
                                    
             else
             {
                 calcWeight(stat,i+1);
                 weightgoal.push_back(weight);
             }
     }
     
}

/*synartisi pou ypologizei anadromika to "varos" enos kyvou, diladi to 
 *posous kyvous exei apo panw tou
*/
int  BlocksState::calcWeight(vector<int> stat, int block)
{
    for(int i = 0; i < stat.size(); i++)
    {
            if(stat[i] == block)
            {
              incrweight();
              calcWeight(stat, i+1);
               
            }           
    }                  
       
}

//kyvoi pou den exoune allo apo panw(tous vazw se ena vector olous)
vector<int> BlocksState::freeBlocks(vector<int> state)
{
            vector<int> free;
            int ii;
            
            for(int i = 1; i <= state.size(); i++)
            {
                //an o kyvos i den yparxei sto state, tote den exei kanenan apo panw tou
                  ii=0;
                  
                  while((i != state[ii]) && ( ii < state.size()))
                  {
                           ii++;
                  }
                  if(ii == state.size())
                  free.push_back(i);                    
            }
            return free;     
}

//elegxei an enas kyvos einai panw sto trapezi
bool BlocksState::IsOnTable(vector<int> state, int block)
{
     if(state[block] == 0)
     return true;
     
     return false;
}

//elegxei an enas kyvos den exei allo apo panw tou
bool BlocksState::IsFreeAbove(vector<int> state, int block)
{
     for(int ii=0; ii < state.size(); ii++)
     {
              if(state[ii] == block)
              return false;
     }
     return true;
}

/*elegxei an enas kyvos einai sti swsti thesi. Diladi an einai sto trapezi,
 *elegxei an kai stin teliki inai sto trapezi. Alliws elegxei oli ti stoiba stin
 *opoia "kathetai" o kyvos gia to an einai swsti 
 */
bool BlocksState::BlockIsCorr(vector<int> state, vector<int> goal, int block)
{
    
     //an einai sto trapezi
     if(IsOnTable(state,block) == true)
     {
        //elegxw kai an sti teliki katastasi einai sto trapezi
        if(IsOnTable(goal, block)== true)
        return true;
        
        else
        return false;
     }
     
     //an einai panw se kyvo/stoiva
     else
     {
         //an einai panw sto swsto kyvo, elegxw kai an aytos einai panw se swsto
         if(state[block] == goal[block])
         return BlockIsCorr(state, goal, state[block]-1);
         
         else return false;
     }
}


bool BlocksState::operator==(const State &state) const
{

	BlocksState &curr = (BlocksState&) state;
	
	vector<int> arg_vect = curr.GetCurrState();
	
	for(int i = 0; i < arg_vect.size(); i++)
	{
		if(arg_vect[i] != curr_state[i])
		return false;
	}
	
	return true;
}

void BlocksState::print() const
{
/*	int i;
	
	cout<<"[";
	for( i = 0; i < curr_state.size() -1; i++)
		cout<<curr_state[i]<<".";
	
	cout<<curr_state[i]<<"]"<<endl;
*/}

/*elegxei kata poso sympiptei i twrini katastasi me tin katastasi stoxou
  
*/
int BlocksState::CloseToGoal(vector<int> curr , vector<int> goal) 
{
    int count = 0;
    
    for(int i = 0; i < curr.size(); i++)
    {
            if(curr[i]!= goal[i])
            count++;
    }
    return count;
    
    
}
/*
 Metraei posoi kyvoi einai swsta topothetimenoi sto trapezi 
*/
int BlocksState::CorrectBlocks(vector<int> curr, vector<int> goal)
{
    int count;
    
    count =0;
    
    for(int i = 0; i < curr.size(); i++)
    {
            if((BlockIsCorr(curr,goal , i) == true ))
            count++;
    }
    
    return count;
}
/*
 elegxei an einai swsta topothetimenoi oi apokatw twn apokatw
*/
int BlocksState::CorrUnder(vector<int> under, vector<int> undergoal)
{
    int count;
    count = under.size();
    
    for(int i = 0; i < under.size(); i++)
    {
            if(under[i]==undergoal[i])
            count--;
    }
    return count;
}

/*eyretiki pou elegxei an enas kyvos einai sti swsti thesi. An einai sti swsti thesi, synexizei.
 *An oxi, elegxei an einai eleytheros apo panw tou, diladi den exei kanenan allo. An nai, 
 *prosthetei sto kostos +1. An exei k allo kyvo apo panw tou, ypologizei to varos, diladi to posous
 *kyvous exei. Ayto voithaei sto na ypologizw to posoi kyvoi prepei na metakinithoun siouraetsi wste
 *na kinithei kai o kyvos pou mas endiaferei. Epipleon prosthetw sto kostos to varos pou exei o 
 *kyvos aytos stin katastasi stoxou, pou shmainei ousiastika posoi kyvoi prepei napane panw tou.
 */
int BlocksState::Lasttry(vector<int> cur, vector<int> goal,vector<int> Weight,vector<int> weightgoal)
{
    int cost = 0;
        
    for(int i = 0; i < cur.size(); i++)
    {
            //an einai se swsti thesi synexizoume
            if(BlockIsCorr(cur,goal , i) == true )
           continue;
          
            else{
                    //an den einai se swsti thesi, elegxw an den exei allo apo panw
                     if(IsFreeAbove( cur,i+1) == true)
                     cost++;
                                    
                     //an exei k allous apo panw, ayksanw to kostos  kata to varos
                     //tou kyvou , kai kata to varos pou exei stin katastasi stoxou              
                     else 
                     {
                          cost += Weight[i];
                          cost += weightgoal[i];
                     }                     
               }         
    }    
    
    
    return cost;
}

/*ayti i eyretiki ypologizei me ti voitheia tis synartisis BlockIsCorr , to an 
 *enas kyvos einai sti swsti thesi.H swsti thesi prosdiorizetai ws tin thesi pou 
 * exei enas kyvos otan vrisketai panw se enan allo pou kai stin teliki einai 
 *apo katw tou , kai otan oloi oi apo katw episis kyvoi einai swsta topothetimenoi.
 * Etsi ypologizetai o arithmos twn "swstwn " kyvwn kai afaireitai apo ton arithmo 
 * twn "lathos" kyvwn.
 */
int BlocksState::HeuristicNo2(vector<int> state, vector<int> goal)
{
    int cost1 = 0,cost2= 0;
    
    for(int i = 0; i < state.size(); i++)
    {
            if( BlockIsCorr(state,goal , i) == true )
            cost1++;
            
            else cost2++;
    }
    
    return cost2-cost1;
}

//destructor
BlocksState::~BlocksState()
{
  
}
