// Class declaration
#include "Sensors.h"
#include "ROS_include.h"

// Wiring
#include "wire_scheme.h"

// Libs used in this file
#include "DallasTemperature.h"
#include <HX711_ADC.h>

// Weight sensor
#define WEIGHT_STABALISING_TIME 2000
#define WEIGHT_CALIBRATION 1700.0
HX711_ADC LoadCell(WEIGHT_DOUT, WEIGHT_SCK);

// Temp sensor
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature temperature(&oneWire);


/*/-- Code --/*/


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

float Sensors::detectSpillage() {
    return analogRead(RAIN_SENS_PIN) / 1024.0;
}

uint8_t Sensors::getStock(uint8_t column) {
    return column;
}

bool Sensors::getLasers(uint8_t gap) {
    return false;
}
