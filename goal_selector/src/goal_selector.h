#ifndef _GOAL_SELECTOR_H_
#define _GOAL_SELECTOR_H_


#include "Astar_searcher.h"

class GoalSelector : public AstarPathFinder{

public:
void selectGoal(Vector3d start_pt);
GridNodePtr dfsFindGoal(GridNodePtr currentPtr);
};



#endif