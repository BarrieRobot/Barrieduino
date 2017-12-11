//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_WIRING_H
#define COFFEE_LOADER_WIRING_H

#include <Arduino.h>

#define LED1_PIN        3
#define LED2_PIN        4

#define ONE_WIRE_PIN    2

#define SERVO_POWER_PIN 3
#define SERVO_START_POS 0
#define SERVO_END_POS   90
#define SERVO_TIMEOUT   1000

#define RFID_TIMEOUT    50

const uint8_t ColdServoPins[] = {9, 10, 11, 12};

#define CUP_DROP_SWITCH 43
#define CUP_DET_SWITCH  40
#define CANISTER_SWITCH 41

#define DISPENSING_ACTUATOR 50
#define CANISTER_ACTUATOR   51

const int8_t CoffeeKeys[] = {A10, A9, A8, A12, A11};

// Also throw in a enum with the result of the keys
/*enum drinks: uint8_t {
    coffee = CoffeeKeys[0],
    can_coffee = CoffeeKeys[1],
    capucchino = CoffeeKeys[2],
    cafe_au_lait = CoffeeKeys[3],
    hot_water = CoffeeKeys[4]
};*/

#define RAIN_SENS_PIN   A0

#define WEIGHT_DOUT 4
#define WEIGHT_SCK  5

#endif //COFFEE_LOADER_WIRING_H
