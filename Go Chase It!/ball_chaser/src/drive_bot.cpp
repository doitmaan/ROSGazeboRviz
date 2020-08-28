#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// This callback function executes whenever a safe_move service is requested
bool handle_request(ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res)
{


    // Create a motor_command object of type geometry_msgs::Twist
    geometry_msgs::Twist motorcmd;
    // Set wheel velocities, forward 
    motorcmd.linear.x = req.linear_x;
    motorcmd.angular.z = req.angular_z;
    // Publish angles to drive the robot
    motor_command_publisher.publish(motorcmd);
    
    // Return a response message
    res.msg_feedback = "Command set received "; 
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize the arm_mover node and create a handle to it
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;

    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_request);
    ROS_INFO("speed command to be sent");

    // Handle ROS communication events
    ros::spin();

    return 0;
}