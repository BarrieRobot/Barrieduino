//
// Created by Casper on 30/11/2017.
//

#include "function_list.h"
#include "wire_scheme.h"
#include <Servo.h>

Servo ColdServo[sizeof(ColdServoPins)];
Servo DiaphragmServo[sizeof(DiaphragmPins)];
uint32_t TServoActivated = 0;

void servo_innit() {
	pinMode(SERVO_POWER_PIN, OUTPUT);

	char buff[65];
	for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
		ColdServo[i].attach(ColdServoPins[i]);

		sprintf(buff, "Arduino: Attaching cold drink servo %u on pin %u", i, ColdServoPins[i]);
		logInfo(buff);
	}
	for (uint8_t i = 0; i < sizeof(DiaphragmPins); ++i) {
		DiaphragmServo[i].attach(DiaphragmPins[i]);

		sprintf(buff, "Arduino: Attaching diaphragm servo %u on pin %u", i, DiaphragmPins[i]);
		logInfo(buff);
	}
	for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
		ColdServo[i].write(SERVO_START_POS);
	}
	delay(50);
	digitalWrite(SERVO_POWER_PIN, LOW);  // The relay is inverted
}

void update_servos() {
	// Reset servo position after SERVO_TIMEOUT milliseconds
	// to allow for the next cold drink to take position
	if (TServoActivated && (millis() > TServoActivated + SERVO_TIMEOUT)) {
		for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
			ColdServo[i].write(SERVO_START_POS);
		}
		TServoActivated = 0;
		logInfo("Deactivated servo");
	}
}

void ejectColdDrink(uint8_t drink) {
	if (drink < sizeof(ColdServoPins)) {
		// Set time the servo was activated to set off reset after SERVO_TIMEOUT
		TServoActivated = static_cast<uint32_t>(millis());
		ColdServo[drink].write(SERVO_END_POS);
		char buff[60];
		sprintf(buff, "Arduino: Activating servo %u on pin %u", drink, ColdServoPins[drink]);
		logInfo(buff);
	} else {
		logWarn("ejectColdDrink: drink is outside of possible range");
	}
}

void moveDiaphragm(uint8_t diaphragm, bool position) {
	if (position) {
        logInfo("Opening diaphragm");
		DiaphragmServo[diaphragm].write(DIAPHRAGM_OPEN_POS);
	} else {
        logInfo("Closing diaphragm");
        DiaphragmServo[diaphragm].write(DIAPHRAGM_CLOSED_POS);
	}
}
