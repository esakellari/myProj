/*        сайеккаяг екисабет 1115200600152      */

#ifndef QUEENSTOBEPLACEDHEURISTIC_H_
#define QUEENSTOBEPLACEDHEURISTIC_H_

#include "../framework/HeuristicFunction.h"
#include "BlocksState.h"

class blocksToBePlacedHeuristic : public HeuristicFunction
{
public:
	blocksToBePlacedHeuristic();
	int getHeuristicValue(const State &state) const;
	virtual ~blocksToBePlacedHeuristic();
};

#endif /*QUEENSTOBEPLACEDHEURISTIC_H_*/
