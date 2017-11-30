//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_COFFEEMACHINE_H
#define COFFEE_LOADER_COFFEEMACHINE_H

#include <stdint-gcc.h>
#include "wire_scheme.h"

#define CANISTER_TIMEOUT    3000
#define DROP_TIMEOUT        500
#define COFFEE_PRESS_TIME   500

class CoffeeMachine {
public:
    bool begin();
    void makeDrink(uint8_t choice);
    void dropCup();
    void rotateCanister();
    void update();

private:
    uint32_t dropStart = 0, canisterRotStart = 0, pressStart = 0;
};


#endif //COFFEE_LOADER_COFFEEMACHINE_H
