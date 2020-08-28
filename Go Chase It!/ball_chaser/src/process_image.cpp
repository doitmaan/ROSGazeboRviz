#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

//Define a global client that can request services

ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget drive_cmd_handler;

    drive_cmd_handler.request.angular_z = ang_z;

    drive_cmd_handler.request.linear_x = lin_x;
    
    
    if (!client.call(drive_cmd_handler)){
        ROS_ERROR("Failed to execute drive command");
    }
}

//This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    

    ROS_INFO("image processing loop ");

     bool found = false;
    int white_pixel = 255;
   
    int size_img = img.height * img.step;

    int side = 0;
    int divisions  = 3;
    for (int i=0; i < size_img; i += divisions)
    {

        

        if ( img.data[i] == white_pixel ) 
        
        {

          
              ROS_INFO("%d  pixel found. ", (int)i);

            if (i % img.step < (img.step/3 * 2))
                drive_robot(0.6, 0); 
                
            else if (i % img.step < img.step/3)
                drive_robot(0.6, 1);
            else
                drive_robot(0.6, -1);




            found = true;



            break;
         }
     }

     if (found == false)
         drive_robot(0, 0);   

}

int main(int argc, char** argv)
{
    ROS_INFO("start");
    ros::init(argc, argv, "process_image");
    ROS_INFO("def handler");
    ros::NodeHandle n;
     ROS_INFO("service call ");


    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");


    ROS_INFO("subscribe to  /my_robot/camera1/rgb/image_raw");
    ros::Subscriber sub1 = n.subscribe("/my_robot/camera1/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}
