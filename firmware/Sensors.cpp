// Class declaration
#include "Sensors.h"

// Wiring
#include "wire_scheme.h"

// Libs used in this file

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

uint16_t Sensors::detectSpillage() {
    return analogRead(RAIN_SENS_PIN);
}
