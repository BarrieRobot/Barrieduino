//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_ROS_INCLUDE_H
#define COFFEE_LOADER_ROS_INCLUDE_H

#include <ros.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/String.h>

// Custom messages
#include "ros_lib/barrieduino/activateOrder.h"
#include "ros_lib/barrieduino/diaphragm.h"
#include "ros_lib/barrieduino/HSL.h"
#include "ros_lib/barrieduino/ledRing.h"

// Custom services
#include "ros_lib/barrieduino/sensorRequest.h"
using barrieduino::sensorRequest;

#endif //COFFEE_LOADER_ROS_INCLUDE_H
