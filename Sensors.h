//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_SENSORS_H
#define COFFEE_LOADER_SENSORS_H

#include <Arduino.h>

class Sensors {
public:
    bool begin();
    float getTemperature();
    float getWeight();
    uint16_t detectSpillage();
};

#endif //COFFEE_LOADER_SENSORS_H
