#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Float32.h>
#include <math.h>
#include <tf/tf.h>
#include <geometry_msgs/Twist.h>

ros::Publisher twist_Pub;
//std_msgs::Float32 msg_sonar;
geometry_msgs::Twist velocidade;
double x = -3;
double y = -1.5;

void odomCallback(const nav_msgs::OdometryConstPtr &msg)
{
    // Instantiate quaternion
    geometry_msgs::Quaternion qt;
    // Instantiate quaternion
    geometry_msgs::Point pt;
    // Copy from received message
    qt = msg->pose.pose.orientation;
    // Define double
    double yaw = tf::getYaw(qt);
    //print to screen result
    std::cout << "Yaw: " << yaw*180/M_PI << std::endl;
    
    pt = msg->pose.pose.position;
    
    double teta = atan((y-pt.y)/(x-pt.x));
    if ((x < pt.x) && (y > pt.y))
        teta = teta + M_PI;
    if ((x < pt.x) && (y < pt.y))
        teta = teta - M_PI;
    
    double delta_teta = 0.5*(teta - yaw);
    velocidade.angular.z = delta_teta;
    velocidade.linear.x = 0;
 //   velocidade.linear.x = 0.2;
    double distancia = sqrt(pow((x-pt.x),2)+pow((y-pt.y),2));
    
    if (delta_teta < 0)
        delta_teta = delta_teta * (-1);
    
    if (delta_teta < 0.01){
        if(distancia >= 0.1)
            velocidade.linear.x = 0.2;
        else
            velocidade.linear.x = 0;
    }
    

//        if(distancia <= 0.3)
//            velocidade.linear.x = 0;

    twist_Pub.publish(velocidade);
}
    
int main(int argc, char **argv)
{
    // Start ROS within this node
    ros::init(argc, argv, "gotopoint");
    // Create node
    ros::NodeHandle nh;
    //Subscribing to odometry topic
    ros::Subscriber odom_sub = nh.subscribe("/vrep/vehicle/odometry",1,odomCallback);
    
    twist_Pub  = nh.advertise<geometry_msgs::Twist>("orientation/twist",1);
    ros::spin();
}