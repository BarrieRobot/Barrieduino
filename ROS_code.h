//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_ROS_CODE_H
#define COFFEE_LOADER_ROS_CODE_H

#include "function_list.h"
#include "ROS_include.h"

// Robot Operating System
ros::NodeHandle nh;

ros::ServiceServer<sensorRequest::Request, sensorRequest::Response> sensorRequest_server("sensorRequest", &sensorRequest);
//ros::ServiceClient<Test::Request, Test::Response> client("test_srv");
//ros::ServiceServer<Test::Request, Test::Response> server("test_srv", &callback);

ros::Subscriber<barrieduino::ledRing> LED_subscriber("LED_progress", &ros_LED);
ros::Subscriber<barrieduino::diaphragm> diaphragm_subscriber("diaphragm", &ros_diaphragm);
ros::Subscriber<barrieduino::activateOrder> activateOrder_sub("activateOrder", &activateOrder);

std_msgs::UInt32 RFID_msg;
ros::Publisher RFID_pub("RFID", &RFID_msg);

/*/-- Function declarations --/*/

void ROS_init ();

#endif //COFFEE_LOADER_ROS_CODE_H
