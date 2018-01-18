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
	const uint8_t closed_angle, open_angle;
	bool position;
	uint32_t transition, afterparty;
	Servo servo;
};
Diaphragm diaphragms[] = {{DiaphragmPins[0], 100, 145, false, 0, 0},	// Cold drinks, pink wire
						  {DiaphragmPins[1], 85, 130, false, 0, 0}};	// Hot drinks, white wire

void servo_init() {
	char buff[65];
	ColdServo.attach(COLD_SERVO_PWM_PIN);
	ColdServo.write(SERVO_START_POS);

	sprintf(buff, "Arduino: Attached cold servo PWM pin to %u", COLD_SERVO_PWM_PIN);
	logInfo(buff);

	uint8_t i = 0;
	for (auto &pin : ColdServoPins) {
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);

		sprintf(buff, "Arduino: Registering cold drink servo %u with enable pin %u", i++, pin);
		logInfo(buff);
	}
	i = 0;
	for (auto &d : diaphragms) {
		d.servo.attach(DiaphragmPins[i]);
		d.servo.write(d.closed_angle);
		d.afterparty = millis();

		sprintf(buff, "Arduino: Attaching diaphragm servo %u on pin %u", i++, DiaphragmPins[i]);
		logInfo(buff);
	}
}

void update_servos() {
	// Reset servo position after SERVO_TIMEOUT milliseconds
	// to allow for the next cold drink to take position
	if (TServoActivated && (millis() > TServoActivated + SERVO_TIMEOUT)) {
		ColdServo.write(SERVO_START_POS);
		if (millis() > TServoActivated + 2*SERVO_TIMEOUT) {
			for (auto &pin : ColdServoPins) {
				digitalWrite(pin, HIGH);
			}
			TServoActivated = 0;	// Reset variable
			logInfo("Deactivated cold drink servo");
		}
	}
	for (auto &d : diaphragms) {
		// If the diaphragm is in transition, linearly interpolate between the values
		if (d.transition) {
			if (d.transition + DIAPHRAGM_DURATION > millis()) {
				int deg;
				if (d.position == 0) {
					// Opening the diaphragm
					deg = map(millis() - d.transition,
							  0, DIAPHRAGM_DURATION,
							  d.closed_angle, d.open_angle
					);
				} else {
					// Closing the diaphragm
					deg = map(millis() - d.transition,
							  0, DIAPHRAGM_DURATION,
							  d.open_angle, d.closed_angle
					);
				}
				d.servo.write(deg);
				/*char buff[200];
				sprintf(buff, "Diaphragm: writing %u deg to pin %u", deg, d.pin);
				logInfo(buff);*/
			} else {
				logInfo("Diaphragm: transition over");
				if (d.position == 0) {
					// Opening the diaphragm
					d.servo.write(d.open_angle);
				} else {
					// Closing the diaphragm
					d.servo.write(d.closed_angle);
				}

				d.afterparty = millis();

				// Set position to new position and reset transition value
				d.position = !d.position;
				d.transition = 0;
			}
		} else if (d.afterparty && d.afterparty + DIAPHRAGM_AFTERPARTY < millis()) {
			d.servo.detach();
			d.afterparty = 0;
		}
	}
}

void ejectColdDrink(uint8_t drink) {
	if (drink < sizeof(ColdServoPins)) {
		// Set time the servo was activated to set off reset after SERVO_TIMEOUT
		TServoActivated = static_cast<uint32_t>(millis());
		digitalWrite(ColdServoPins[drink], LOW);
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
	diaphragms[diaphragm].servo.attach(DiaphragmPins[diaphragm]);
	char buff[25];
	if (position) {
		sprintf(buff, "Opening diaphragm %u", diaphragm);
	} else {
		sprintf(buff, "Closing diaphragm %u", diaphragm);
	}
	logInfo(buff);
}
