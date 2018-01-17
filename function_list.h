//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_FUNCTION_LIST_H
#define COFFEE_LOADER_FUNCTION_LIST_H

#include "ROS_include.h"

// Servos
void servo_init();
void update_servos();
void ejectColdDrink(uint8_t drink);
void moveDiaphragm(uint8_t diaphragm, bool position);

// RFID reader
uint32_t getTagInfo();

void activateOrder(const barrieduino::activateOrder message);
void ros_diaphragm(const barrieduino::diaphragm message);
void ros_LED(const barrieduino::ledRing message);
void sensorRequest(const sensorRequest::Request &request, sensorRequest::Response &response);
void logInfo(const char* message);
void logWarn(const char* message);

#endif //COFFEE_LOADER_FUNCTION_LIST_H
