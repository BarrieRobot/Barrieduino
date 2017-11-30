//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_SENSORS_H
#define COFFEE_LOADER_SENSORS_H

#include "wire_scheme.h"

#include "DallasTemperature.h"
#include <HX711_ADC.h>

// Weight sensor
#define WEIGHT_STABALISING_TIME 2000
#define WEIGHT_CALIBRATION 1700.0
HX711_ADC LoadCell(WEIGHT_DOUT, WEIGHT_SCK);
// Temp sensor
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature temperature(&oneWire);

class Sensors {
public:
    bool begin();
    float getTemperature();
    float getWeight();
    uint16_t detectSpillage();
};


#endif //COFFEE_LOADER_SENSORS_H
