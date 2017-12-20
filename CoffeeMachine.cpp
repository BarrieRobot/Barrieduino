//
// Created by Casper on 30/11/2017.
//

#include "CoffeeMachine.h"
#include "function_list.h"

bool CoffeeMachine::begin() {
    for (int i = 0; i < sizeof(CoffeeKeys); ++i) {
        digitalWrite(CoffeeKeys[i], HIGH);
        pinMode(CoffeeKeys[i], OUTPUT);
    }

    pinMode(CUP_DROP_SWITCH, INPUT_PULLUP);
    pinMode(CUP_DET_SWITCH, INPUT_PULLUP);
    pinMode(CANISTER_SWITCH, INPUT_PULLUP);

    pinMode(DISPENSING_ACTUATOR, OUTPUT);
    digitalWrite(DISPENSING_ACTUATOR, HIGH);
    pinMode(CANISTER_ACTUATOR, OUTPUT);
    digitalWrite(CANISTER_ACTUATOR, HIGH);
    return true;
}

void CoffeeMachine::makeDrink(uint8_t choice) {
    pressStart = millis();
    char buffer[60];
    sprintf(buffer, "Activating button %u from the coffeemachine", choice);
    logInfo(buffer);
    digitalWrite(CoffeeKeys[choice], LOW);
}

void CoffeeMachine::dropCup() {
    logInfo("Starting cup dispension");

    // If no cups are present, rotate the canister to a new column
    if (digitalRead(CUP_DET_SWITCH) == false) {
        rotateCanister();
    } else {
        // Turn on dispensing motor
        digitalWrite(DISPENSING_ACTUATOR, LOW);
        dropStart = millis();
    }
}

void CoffeeMachine::rotateCanister() {
    logInfo("Cups from the current canister are depleted, rotating to the next");
    canisterRotStart = millis();
    digitalWrite(CANISTER_ACTUATOR, LOW);
}

void CoffeeMachine::update() {
    /*char buffer[200];
    sprintf(buffer, "pressStart = %lu, millis() = %lu, diff = %lu",
            pressStart, millis(), (millis() - pressStart));
    logInfo(buffer);*/

    // Reset keypress of coffee machine after COFFEE_PRESS_TIME
    if (pressStart && pressStart + COFFEE_PRESS_TIME < millis()) {
        pressStart = 0;
        for (uint8_t i = 0; i < sizeof(CoffeeKeys); ++i) {
            digitalWrite(CoffeeKeys[i], HIGH);
        }
        logInfo("Coffee key release");
    }

    /*char buffer[200];
    sprintf(buffer, "canisterRotStart = %lu, millis() = %lu, diff = %lu, read = %u\ncanisterSwitchStart = %lu, diff = %lu",
            canisterRotStart, millis(), (millis() - canisterRotStart), digitalRead(CANISTER_SWITCH),
            canisterSwitchStart, (millis() - canisterSwitchStart));
    logInfo(buffer);*/

    // Check if the canister holder has completed the first part of its rotation
    if (canisterRotStart && canisterRotStart + CANISTER_TIMEOUT1 < millis() && digitalRead(CANISTER_SWITCH) == false) {
        canisterRotStart = 0;
        canisterSwitchStart = millis();
        logInfo("Completed first part of rotation!");
    }
    // Canister holder is in position
    else if (canisterSwitchStart && canisterSwitchStart + CANISTER_TIMEOUT2 < millis() && digitalRead(CANISTER_SWITCH) == true) {
        canisterSwitchStart = 0;
        // Turn off canister rotation motor
        digitalWrite(CANISTER_ACTUATOR, HIGH);
        // Throw warning if no cups are detected still. TODO: insert delay needed for cups to fall through
        if (digitalRead(CUP_DET_SWITCH) == false) {
            logWarn("Cup stock is completely depleted");
        } else {
            logInfo("Cups restocked by next canister");
            // Now that new stock is present, drop the cup
            dropCup();
        }
    }

    // Check if a cup has been dispensed yet
    if (dropStart && dropStart + DROP_TIMEOUT < millis() && digitalRead(CUP_DROP_SWITCH)) {
        dropStart = 0;
        digitalWrite(DISPENSING_ACTUATOR, HIGH);
        logInfo("Cup has been dispensed");
    }
}
