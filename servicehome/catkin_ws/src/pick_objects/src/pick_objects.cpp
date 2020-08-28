#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>


float dropOff[3] = {5, 2.5, 1.0};


float pickUp[3] = {1.0, 4.0, 1.0};


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "pick_objects");

  MoveBaseClient ac("move_base", true);

  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

 ROS_INFO("setting up the position and oreintation");
 goal.target_pose.pose.orientation.w = pickUp[2] ;



  goal.target_pose.pose.position.x = pickUp[0];
  goal.target_pose.pose.position.y = pickUp[1];
  

  ROS_INFO("transmitting commands");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
     
    {


     ROS_INFO("@ pickStation");


     ros::Duration(5.0).sleep();

    goal.target_pose.pose.orientation.w = dropOff[2];

     ROS_INFO("setting up droping positin and oreintation ");


     goal.target_pose.pose.position.x = dropOff[0];
     goal.target_pose.pose.position.y = dropOff[1];
     ROS_INFO("transmiting commands ");
     ac.sendGoal(goal);
    
    ROS_INFO("waiying on results ");
     ac.waitForResult();
    
//     ROS_INFO(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED);

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
     {ROS_INFO("@droping station");
     ros::Duration(5.0).sleep();}
  
  }
  return 0;
}