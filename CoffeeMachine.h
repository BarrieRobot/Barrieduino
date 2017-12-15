//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_COFFEEMACHINE_H
#define COFFEE_LOADER_COFFEEMACHINE_H

#include "wire_scheme.h"

#define CANISTER_TIMEOUT1   3000
#define CANISTER_TIMEOUT2   500
#define DROP_TIMEOUT        1000
#define COFFEE_PRESS_TIME   500

class CoffeeMachine {
public:
    bool begin();
    void makeDrink(uint8_t choice);
    void dropCup();
    void update();

private:
    void rotateCanister();

private:
    uint32_t dropStart = 0, canisterRotStart = 0, canisterSwitchStart = 0, pressStart = 0;
};


#endif //COFFEE_LOADER_COFFEEMACHINE_H
