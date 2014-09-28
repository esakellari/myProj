/*        сайеккаяг екисабет 1115200600152      */

#include "blocksToBePlacedHeuristic.h"

blocksToBePlacedHeuristic::blocksToBePlacedHeuristic()
{
}

int blocksToBePlacedHeuristic::getHeuristicValue(const State &state) const
{
	vector<int> curr, goal,under,undergoal,curh,goalh,Weightcur,Weightgoal;
	
    BlocksState &curr_State = (BlocksState &) state;

    curr = curr_State.GetCurrState();
	goal = curr_State.GetGoalState();
	under = curr_State.GetUnderneath();
	undergoal = curr_State.GetUndergoal();
	Weightcur = curr_State.Getweight();
	Weightgoal = curr_State.Getweightgoal();
    
/*---------------------------------------------------------------------------*/   
    //trexei veltista mexri 14 kyvous
	//return curr_State.CloseToGoal(curr, goal);
	
	
/*----------------------------------------------------------------------------*/	
    /*return curr_State.CorrUnder(under,undergoal)+
             curr_State.CloseToGoal(curr, goal);
    */
    
/*----------------------------------------------------------------------------*/    
    //return curr_State.Lasttry(curr, goal,Weightcur,Weightgoal);
      
/*----------------------------------------------------------------------------*/      
      //me ayto to "athroisma " eyretikwn trexei gia mexri 37 kybous, kai veltista mexri 17 kyvoys
      return curr_State.Lasttry(curr, goal,Weightcur,Weightgoal)+curr_State.HeuristicNo2(curr,goal)
      +curr_State.CloseToGoal(curr, goal);
     
/*----------------------------------------------------------------------------*/      
    //me ayti tin eyretiki trexei veltista gia mexri 29 kyvous
    //return curr_State.HeuristicNo2(curr,goal);
                                           
}

blocksToBePlacedHeuristic::~blocksToBePlacedHeuristic()
{
}
