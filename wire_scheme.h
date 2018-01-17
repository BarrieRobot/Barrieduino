//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_WIRING_H
#define COFFEE_LOADER_WIRING_H

#include <Arduino.h>

#define RFID_TIMEOUT    50

#define LED1_PIN        20
#define LED2_PIN        21

#define ONE_WIRE_PIN    2

#define DIAPHRAGM_CLOSED_POS    50
#define DIAPHRAGM_OPEN_POS      140
#define DIAPHRAGM_DURATION      1000

#define SERVO_START_POS 10
#define SERVO_END_POS   70
#define SERVO_TIMEOUT   1000

#define COLD_SERVO_PWM_PIN 33
//#define INTERRUPT_PIN_COLD 3
const uint8_t ColdServoPins[] = {31, 29, 30};
const uint8_t ColdDetectionPins[] = {26, 27, 28};
const uint8_t DiaphragmPins[] = {16, 17};

#define DIAPHRAGM_LED_OFFSET 5

#define CUP_DROP_SWITCH 52
#define CUP_DET_SWITCH  51
#define CANISTER_SWITCH 53

#define DISPENSING_ACTUATOR A7
#define CANISTER_ACTUATOR   A6

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
