/*        сайеккаяг екисабет 1115200600152      */

#ifndef _BLOCKSGOALTEST_
#define _BLOCKSGOALTEST_

#include <vector>
#include "../framework/GoalTest.h"
#include "BlocksState.h"

using namespace std;

class BlocksGoalTest : public GoalTest
{
      private:
              
              
      public:
             BlocksGoalTest();
             bool isGoalState(const State &state) const;
             
             ~BlocksGoalTest();
      
      
};


#endif
