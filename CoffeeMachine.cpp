//
// Created by Casper on 30/11/2017.
//

#include <led_sysdefs.h>
#include "CoffeeMachine.h"
#include "ROS_code.h"

bool CoffeeMachine::begin() {
    for (int i = 0; i < sizeof(CoffeeKeys); ++i) {
        pinMode(CoffeeKeys[i], OUTPUT);
    }

    pinMode(CUP_DROP_SWITCH, INPUT);
    pinMode(CUP_DETE_SWITCH, INPUT);
    pinMode(CANISTER_SWITCH, INPUT);

    pinMode(DISPENSION_ACTUATOR, OUTPUT);
    pinMode(CANISTER_ACTUATOR, OUTPUT);
    return true;
}

void CoffeeMachine::makeDrink(uint8_t choice) {
    pressStart = 0;
    digitalWrite(CoffeeKeys[choice], HIGH);
}

void CoffeeMachine::dropCup() {
    // TODO: check if digitalRead should be inverted
    if (digitalRead(CUP_DETE_SWITCH)) {
        this->rotateCanister();
    }

    dropStart = millis();

}

void CoffeeMachine::rotateCanister() {
    nh.loginfo("Cups from the current canister are depleted, rotating to the next");
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

    // Check if the canister holder has completed its rotation
    // TODO: check if digitalRead should be inverted
    if (canisterRotStart && canisterRotStart + CANISTER_TIMEOUT > millis() && digitalRead(CANISTER_SWITCH)) {
        canisterRotStart = 0;
        // TODO: check if digitalRead should be inverted
        if (!digitalRead(CUP_DETE_SWITCH)) {
            digitalWrite(CANISTER_ACTUATOR, LOW);
        } else {
            nh.logwarn("Cup stock is completely depleted");
        }
    }

    // Check if a cup has been dispensed yet
    // TODO: check if digitalRead should be inverted
    if (dropStart && dropStart + DROP_TIMEOUT > millis() && digitalRead(CUP_DROP_SWITCH)) {
        dropStart = 0;
        digitalWrite(DISPENSION_ACTUATOR, LOW);
    }
}
