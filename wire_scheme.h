//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_WIRING_H
#define COFFEE_LOADER_WIRING_H

#include <Arduino.h>

#define RFID_TIMEOUT    50

#define LED1_PIN        3
#define LED2_PIN        4

#define ONE_WIRE_PIN    2

#define DIAPHRAGM_CLOSED_POS    50
#define DIAPHRAGM_OPEN_POS      140
#define DIAPHRAGM_DURATION      1000

#define SERVO_START_POS 10
#define SERVO_END_POS   70
#define SERVO_TIMEOUT   1000

#define COLD_SERVO_PWM_PIN 9
const uint8_t ColdServoPins[] = {17, 18, 19};
const uint8_t DiaphragmPins[] = {22, 23};
struct Diaphragm {
    const uint8_t pin;
    bool position;
    uint32_t transition;
};
Diaphragm diaphragms[] = {{DiaphragmPins[0], false, 0}, {DiaphragmPins[1], false, 0}};

#define CUP_DROP_SWITCH 43
#define CUP_DET_SWITCH  40
#define CANISTER_SWITCH 41

#define DISPENSING_ACTUATOR 50
#define CANISTER_ACTUATOR   51

//const uint8_t CoffeeKeys[] = {A10, A9, A8, A12, A11};
const uint8_t CoffeeKeys[] = {A12, A11, A10, A9, A8};

// Also throw in a enum with the result of the keys
/*enum drinks: uint8_t {
    coffee = CoffeeKeys[0],
    can_coffee = CoffeeKeys[1],
    capucchino = CoffeeKeys[2],
    cafe_au_lait = CoffeeKeys[3],
    hot_water = CoffeeKeys[4]
};*/

#define RAIN_SENS_PIN   A0

#define WEIGHT_DOUT 26
#define WEIGHT_SCK  27

#endif //COFFEE_LOADER_WIRING_H
