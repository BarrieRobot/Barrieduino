//
// Created by Casper on 30/11/2017.
//

#include "Sensors.h"

bool Sensors::begin() {
    temperature.begin();

    pinMode(RAIN_SENS_PIN, INPUT);

    LoadCell.begin();
    LoadCell.start(WEIGHT_STABALISING_TIME);
    LoadCell.setCalFactor(WEIGHT_CALIBRATION);
    return true;
}

float Sensors::getTemperature() {
    temperature.requestTemperatures();
    return temperature.getTempCByIndex(0);
}

float Sensors::getWeight() {
    return LoadCell.getData();
}

uint16_t Sensors::detectSpillage() {
    return analogRead(RAIN_SENS_PIN);
}
