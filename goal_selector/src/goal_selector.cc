
#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Vector3.h>
#include <nav_msgs/Path.h>

#include "goal_selector.h"
#include <vector>

ros::Publisher goalPublisher;

inline void GoalSelector::selectGoal(Vector3d start_pt){
    Vector3i start_idx = coord2gridIndex(start_pt);
    start_pt = gridIndex2coord(start_idx);

    startPtr = new GridNodePtr(start_idx, start_pt);
    
    GridNodePtr goal = dfsFindGoal(startPtr);
}

GridNodePtr dfsFindGoal(GridNodePtr currentPtr){
    
    int hasNeighborObs = 0
    for (int i = -2; i <= 2; i++)
        for (int j = -2; j <= 2; j++)
            for (int k = -2; k <= 2; k++){
                if (!(i == j && j == k && i == 0)){
                    idx_nerigbor(0) = currentPtr->index(0) + i;
                    idx_nerigbor(1) = currentPtr->index(1) + j;
                    idx_nerigbor(2) = currentPtr->index(2) + k;
                    if(isOccupied(currentPtr)){
                        hasNeighborObs = 1;
                        break;
                    }
                }
    }
    if(!hasNeighborObs) return currentPtr;

    Vector3d coord_nerigbor;
    Vector3i idx_nerigbor;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            {
                if (!(i == j && i == 0))  //遍历当前节点为中心的平面上的3*3格子,除了自身
                {
                    idx_nerigbor(0) = currentPtr->index(0) + i;
                    idx_nerigbor(1) = currentPtr->index(1) + j;
                    idx_nerigbor(2) = currentPtr->index(2);
                    coord_nerigbor = gridIndex2coord(idx_nerigbor);

                    if ((coord_nerigbor(0) >= gl_xl && coord_nerigbor(0) <= gl_xu)  //判断当前nerigbor是否有效
                    && (coord_nerigbor(1) >= gl_yl && coord_nerigbor(1) <= gl_yu) 
                    && (coord_nerigbor(2) >= gl_zl && coord_nerigbor(2) <= gl_zu))
                    {
                        idx_nerigbor = coord2gridIndex(coord_nerigbor);

                        if (isFree(idx_nerigbor))   //查询nerigbor所在栅格是否被障碍物占用,若已经存在障碍,则放弃该nerigbor
                        {
                            GridNodePtr neighborPtr = new GridNode(idx_nerigbor, coord_nerigbor);
                            dfsFindGoal(neighborPtr);
                        }
                    }
                }
            }
}

int main(){
    ros::init(argc, argv, "goal_selector");
    ros::NodeHandle n("~");
    goalPublisher = n.advertise<geometry_msgs::PoseStamped>("goal_selector", 10);

    ros::spin();
    return 0;
}