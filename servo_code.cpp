//
// Created by Casper on 30/11/2017.
//

#include "function_list.h"
#include "wire_scheme.h"
#include <Servo.h>

Servo ColdServo;
uint32_t TServoActivated = 0;

struct Diaphragm {
	const uint8_t pin;
	bool position;
	uint32_t transition;
	Servo servo;
};
Diaphragm diaphragms[] = {{DiaphragmPins[0], false, 0}, {DiaphragmPins[1], false, 0}};

void servo_innit() {
	ColdServo.attach(COLD_SERVO_PWM_PIN);
	ColdServo.write(SERVO_START_POS);

	char buff[65];
	for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
		pinMode(ColdServoPins[i], OUTPUT);
		digitalWrite(ColdServoPins[i], LOW);

		sprintf(buff, "Arduino: Registering cold drink servo %u with enable pin %u", i, ColdServoPins[i]);
		logInfo(buff);
	}
	for (uint8_t i = 0; i < sizeof(DiaphragmPins); ++i) {
		diaphragms[i].servo.attach(DiaphragmPins[i]);
		diaphragms[i].servo.write(DIAPHRAGM_CLOSED_POS);

		sprintf(buff, "Arduino: Attaching diaphragm servo %u on pin %u", i, DiaphragmPins[i]);
		logInfo(buff);
	}
}

void update_servos() {
	// Reset servo position after SERVO_TIMEOUT milliseconds
	// to allow for the next cold drink to take position
	if (TServoActivated && (millis() > TServoActivated + SERVO_TIMEOUT)) {
		ColdServo.write(SERVO_START_POS);
		if (millis() > TServoActivated + 2*SERVO_TIMEOUT) {
			for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
				digitalWrite(ColdServoPins[i], LOW);
			}
			TServoActivated = 0;	// Reset variable
			logInfo("Deactivated cold drink servo");
		}
	}
	for (auto &d : diaphragms) {
		// If the diaphragm is in transition, linearly interpolate between the values
		if (d.transition) {
			if (millis() - d.transition < DIAPHRAGM_DURATION) {
				int deg;
				if (d.position == 0) {
					// Opening the diaphragm
					deg = map(millis() - d.transition,
							  0, DIAPHRAGM_DURATION,
							  DIAPHRAGM_CLOSED_POS, DIAPHRAGM_OPEN_POS
					);
				} else {
					// Closing the diaphragm
					deg = map(millis() - d.transition,
							  0, DIAPHRAGM_DURATION,
							  DIAPHRAGM_OPEN_POS, DIAPHRAGM_CLOSED_POS
					);
				}
				d.servo.write(deg);
			} else {
				if (d.position == 0) {
					// Opening the diaphragm
					d.servo.write(DIAPHRAGM_OPEN_POS);
				} else {
					// Closing the diaphragm
					d.servo.write(DIAPHRAGM_CLOSED_POS);
				}

				d.transition = 0;
			}
		}
	}
}

void ejectColdDrink(uint8_t drink) {
	if (drink < sizeof(ColdServoPins)) {
		// Set time the servo was activated to set off reset after SERVO_TIMEOUT
		TServoActivated = static_cast<uint32_t>(millis());
		digitalWrite(ColdServoPins[drink], HIGH);
		ColdServo.write(SERVO_END_POS);
		char buff[60];
		sprintf(buff, "Arduino: Activating servo %u with pin %u", drink, ColdServoPins[drink]);
		logInfo(buff);
	} else {
		logWarn("ejectColdDrink: drink is outside of possible range");
	}
}

void moveDiaphragm(uint8_t diaphragm, bool position) {
	// Sanity checks
	if (diaphragms[diaphragm].position == position) {
		logInfo("Diaphragm already in position");
		return;
	} else if (diaphragms[diaphragm].transition) {
		logInfo("Diaphragm still in transition");
	}

	// Set start of transition to current time
	diaphragms[diaphragm].transition = millis();
	char buff[25];
	if (position) {
		sprintf(buff, "Opening diaphragm %d", diaphragm);
	} else {
		sprintf(buff, "Closing diaphragm %d", diaphragm);
	}
	logInfo(buff);
}
