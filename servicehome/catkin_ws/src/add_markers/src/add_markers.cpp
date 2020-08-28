#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"
#include <math.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>


//Positions and thresholds
float thresh[2] = {1, 1};


//Flags
bool pickAcomplished = false;
bool dropAcomplished = false;

void ServiceTopicallreverse(const nav_msgs::Odometry::ConstPtr& msg)
{ 
  
 

  if ((sqrt(pow(msg->pose.pose.position.x - 1.0, 2) + pow( msg->pose.pose.position.y - 4.0, 2)) ) <= thresh[0] && dropAcomplished==false ) {
    pickAcomplished = true;
 }
  
   if ((sqrt(pow(msg->pose.pose.position.x - 5.0, 2) + pow( msg->pose.pose.position.y - 2.5, 2)) )< thresh[1] && pickAcomplished == true ) {
    dropAcomplished = true;
    
  }
  

}

int main( int argc, char** argv )
{
  
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Publisher markerPublisher = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
// ros::Subscriber odoSubscriber = n.subscribe("odom", 1000, chatterCallback);
  
  
  ros::Subscriber odoSubscriber = n.subscribe("odom", 1000, ServiceTopicallreverse);



  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CYLINDER;

  while (ros::ok())
  {  
    
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    marker.ns = "basic_shapes";
    marker.id = 0;

    marker.type = shape;

    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x = 1.0;
    marker.pose.position.y = 4.0;
    marker.pose.position.z = 0;
    
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (markerPublisher.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("missing a subscriber to the marker");
      sleep(1);
    }
    
   markerPublisher.publish(marker);
    
   
    ROS_INFO("pickup not accomplished ");

    while( pickAcomplished == false ){
      
      ros::spinOnce(); 
      
    }
      
      
     ROS_INFO("left the while loop ");
   
   if(pickAcomplished)
   {
     
    

    marker.action = visualization_msgs::Marker::DELETE;
     
    markerPublisher.publish(marker);
     
    ROS_INFO("the marker is deleted");
    
   }  
    
    
   ros::Duration(10.0).sleep();
    
    ROS_INFO("lentering the scond loop "); 
    
    while( pickAcomplished == true && dropAcomplished==false  ){
      
      ros::spinOnce(); 
      
    }
    
   

    if (dropAcomplished)
   {
     
    
     marker.pose.orientation.w = 1.0;
     
    marker.pose.position.x = 5.0;

    marker.pose.position.y = 2.5;
    
    marker.action = visualization_msgs::Marker::ADD;

    
    markerPublisher.publish(marker);
     
   

    ROS_INFO("DONE the first marker , the second marker to be added");
     
     
    ros::Duration(10.0).sleep();
   }  
    return 0;
  }
 
}