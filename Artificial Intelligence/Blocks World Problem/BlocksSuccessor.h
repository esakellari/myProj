/*        сайеккаяг екисабет 1115200600152      */

#ifndef _BLOCKSSUCCESSOR_
#define _BLOCKSSUCCESSOR_

#include <list>
#include <sstream>

#include "../framework/SuccessorFunction.h"
#include "BlocksState.h"



#define ON_TABLE 1

using namespace std;

class BlocksSuccessor : public SuccessorFunction
{
      private:
              int MoveTopCube(vector<int> curr,int pos, BlocksState &parent,BlocksState *child);
              
      public:
             BlocksSuccessor();
     	     void getSuccessors(const State &state, list<Successor*> *succ);
             ~BlocksSuccessor();
      
};

#endif
