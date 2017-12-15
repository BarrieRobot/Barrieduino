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
void ejectColdDrink(uint8_t drink);

// RFID reader
uint32_t getTagInfo();

void activateOrder(const beginner_tutorials::activateOrder message);
void logInfo(const char* message);
void logWarn(const char* message);

#endif //COFFEE_LOADER_FUNCTION_LIST_H
