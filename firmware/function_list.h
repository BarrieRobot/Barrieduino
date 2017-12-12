//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_FUNCTION_LIST_H
#define COFFEE_LOADER_FUNCTION_LIST_H

#include "ROS_include.h"

// LED ring
void LED_innit();
inline uint16_t promilleToFrac(uint16_t promille);
void updateProgress(uint16_t promille);
void FastLED_show();

// Servos
void servo_innit();
void update_servos();
void ejectColdDrink(uint8_t number);

// RFID reader
uint32_t getTagInfo();

void logInfo(const char* message);
void logWarn(const char* message);
//void callback(const Test::Request &req, Test::Response &res);

#endif //COFFEE_LOADER_FUNCTION_LIST_H
