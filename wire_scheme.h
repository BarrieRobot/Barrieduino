//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_WIRING_H
#define COFFEE_LOADER_WIRING_H

#include <Arduino.h>

#define LED1_PIN		3
#define LED2_PIN		4

#define ONE_WIRE_PIN    2

#define SERVO_POWER_PIN 3
#define SERVO_START_POS 0
#define SERVO_END_POS   90
#define SERVO_TIMEOUT   1000

#define RFID_TIMEOUT    50

const uint8_t ColdServoPins[] = {9, 10, 11, 12};

#endif //COFFEE_LOADER_WIRING_H
