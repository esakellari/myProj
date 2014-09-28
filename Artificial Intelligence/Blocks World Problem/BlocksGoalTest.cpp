/*        сайеккаяг екисабет 1115200600152      */

#include "BlocksGoalTest.h"

BlocksGoalTest::BlocksGoalTest()
{
}

bool BlocksGoalTest::isGoalState(const State &state) const
{
     vector<int> curr,goal;
     BlocksState &st = (BlocksState &)state;
     curr = st.GetCurrState();
     goal = st.GetGoalState();

     
     for(int i = 0; i < goal.size(); i++)
     {

             if(curr[i]!= goal[i])
             return false;
     }
     
     return true;
}

BlocksGoalTest::~BlocksGoalTest()
{}
