//
// Created by Casper on 30/11/2017.
//

#include <led_sysdefs.h>
#include "CoffeeMachine.h"
#include "function_list.h"

bool CoffeeMachine::begin() {
    for (int i = 0; i < sizeof(CoffeeKeys); ++i) {
        pinMode(CoffeeKeys[i], OUTPUT);
    }

    pinMode(CUP_DROP_SWITCH, INPUT_PULLUP);
    pinMode(CUP_DET_SWITCH, INPUT_PULLUP);
    pinMode(CANISTER_SWITCH, INPUT_PULLUP);

    pinMode(DISPENSING_ACTUATOR, OUTPUT);
    pinMode(CANISTER_ACTUATOR, OUTPUT);
    return true;
}

void CoffeeMachine::makeDrink(uint8_t choice) {
    pressStart = 0;
    digitalWrite(CoffeeKeys[choice], HIGH);
}

void CoffeeMachine::dropCup() {
    // If no cups are present, rotate the canister to a new column
    if (digitalRead(CUP_DET_SWITCH)) {
        rotateCanister();
    } else {
        // Turn on dispensing motor
        digitalWrite(DISPENSING_ACTUATOR, HIGH);
        dropStart = millis();
    }
}

void CoffeeMachine::rotateCanister() {
    logInfo("Cups from the current canister are depleted, rotating to the next");
    canisterRotStart = millis();
    digitalWrite(CANISTER_ACTUATOR, HIGH);
}

void CoffeeMachine::update() {
    // Reset keypress of coffee machine after COFFEE_PRESS_TIME
    if (pressStart && pressStart + COFFEE_PRESS_TIME > millis()) {
        for (uint8_t j = 0; j < sizeof(CoffeeKeys); ++j) {
            digitalWrite(CoffeeKeys[i], LOW);
        }
    }

    // Check if the canister holder has completed the first part of its rotation
    if (canisterRotStart && canisterRotStart + CANISTER_TIMEOUT1 > millis() && digitalRead(CANISTER_SWITCH) == true) {
        canisterRotStart = 0;
        canisterSwitchStart = millis();
    }
    // Canister holder is in position
    else if (canisterSwitchStart && canisterSwitchStart + CANISTER_TIMEOUT2 > millis() && digitalRead(CANISTER_SWITCH) == false) {
        canisterSwitchStart = 0;
        // Turn off canister rotation motor
        digitalWrite(CANISTER_ACTUATOR, LOW);
        // Throw warning if no cups are detected still. TODO: check if delay is needed for cups to fall through
        if (digitalRead(CUP_DET_SWITCH) == true) {
            logWarn("Cup stock is completely depleted");
        } else {
            // Now that new stock is present, drop the cup
            dropCup();
        }
    }

    // Check if a cup has been dispensed yet
    if (dropStart && dropStart + DROP_TIMEOUT > millis() && digitalRead(CUP_DROP_SWITCH)) {
        dropStart = 0;
        digitalWrite(DISPENSING_ACTUATOR, LOW);
    }
}
