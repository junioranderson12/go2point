#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <geometry_msgs/Quaternion.h>

    
//Assim que o subscritor receber uma mensagem ela será impressa
void odomCallback(const nav_msgs::OdometryConstPtr &msg)
{
    // Instantiate quaternion
    geometry_msgs::Quaternion qt;
    // Copy from received message
    qt = msg->pose.pose.orientation;
    // Define double
    double yaw = tf::getYaw(qt);
    //print to screen result
    std::cout << "Yaw: " << yaw*180/M_PI << std::endl;
    
}

int main(int argc, char **argv)
{
    // Start ROS within this node
    ros::init(argc, argv, "quat2yaw");
    // Create node
    ros::NodeHandle nh;
    //Subscribing to odometry topic
    ros::Subscriber odom_sub = nh.subscribe("/vrep/vehicle/odometry",1,odomCallback);
    
    ros::spin();
    
}