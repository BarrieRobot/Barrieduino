//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_ROS_CODE_H
#define COFFEE_LOADER_ROS_CODE_H

#include "function_list.h"
#include "ROS_include.h"

// Robot Operating System
ros::NodeHandle nh;

//ros::ServiceClient<Test::Request, Test::Response> client("test_srv");
//ros::ServiceServer<Test::Request, Test::Response> server("test_srv", &callback);

ros::Subscriber<std_msgs::UInt16> LED_subscriber("LED_progress", &updateProgress);
ros::Subscriber<std_msgs::UInt8> ColdDrink_subscriber("cold_drinks", &ejectColdDrink);

std_msgs::UInt32 RFID_msg;
ros::Publisher RFID_pub("RFID", &RFID_msg);

/*/-- Function declarations --/*/

void ROS_init ();

#endif //COFFEE_LOADER_ROS_CODE_H