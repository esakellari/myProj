/*        сайеккаяг екисабет 1115200600152      */

#ifndef _BLOCKS_STATE_
#define _BLOCKS_STATE_

#include <string>
#include <vector>

#include "../datastructures/Point.h"

#include "../framework/State.h"

using namespace std;

class BlocksState : public State
{
      private:
              vector<int> init_state;
              vector<int> goal_state;
              vector<int> curr_state;
              vector<int> underneath;
              vector<int> undergoal; 
              vector<int> weightcur;
              vector<int> weightgoal;
              
              int height;
              int weight;
              int calcWeight(vector<int> stat, int i); 
      public:
             
             //constructor
             BlocksState(vector<int> Init,vector<int> Goal);
             
             //accessors
             vector<int> GetGoalState() const;
             vector<int> GetInitState() const;
             vector<int> GetCurrState() const;
             vector<int> GetUnderneath() const;
             vector<int> GetUndergoal() const;
             vector<int> Getweight() const;
             vector<int> Getweightgoal() const;
             int GetH();
             
             //mutators
             void SetCurrState(vector<int> curr);
             void SetGoalState(vector<int> goal);
             void SetInitState(vector<int> init);
             void SetUnderneath(vector<int> stat);
             void SetUndergoal(vector<int> goal);
             void SetStacks();
             void Setweightcur(vector<int> stat,vector<int> weight);
             void Setweightgoal(vector<int> stat, vector<int> weight);
             void SetH(int i);
             void SetW(int i);
         
             
             bool IsOnTable(vector<int> state, int block);
             bool IsFreeAbove(vector<int> state, int block);
             bool BlockIsCorr(vector<int> state, vector<int> goal, int block);
             vector<int> freeBlocks(vector<int> state);
             
	         bool operator==(const State &state) const;
	         void print() const;
	         
	         void incrweight();
            
            //heuristics
             int CloseToGoal(vector<int> curr,vector<int> goal);
             int CorrectBlocks(vector<int> curr, vector<int> goal);
             int CorrUnder(vector<int> under, vector<int> undergoal);
             int Lasttry(vector<int> cur, vector<int> goal, vector<int> Weight,vector<int> weightgoal);
             int HeuristicNo2(vector<int> state, vector<int> goal);                        
              
              
             //destructor
             ~BlocksState();
             
      
};





#endif
