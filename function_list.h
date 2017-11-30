//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_FUNCTION_LIST_H
#define COFFEE_LOADER_FUNCTION_LIST_H

#include "ROS_include.h"

void LED_innit();
inline uint16_t promilleToFrac(uint16_t promille);
void updateProgress(const std_msgs::UInt16 &cmd_msg);

// Servos
void servo_innit();
void update_servos();
void ejectColdDrink(const std_msgs::UInt8 &cmd_msg);

// RFID reader
uint32_t getTagInfo();

void callback(const Test::Request &req, Test::Response &res);

#endif //COFFEE_LOADER_FUNCTION_LIST_H
