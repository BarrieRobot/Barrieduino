//
// Created by Casper on 30/11/2017.
//

#include "function_list.h"
#include "wire_scheme.h"
#include <Servo.h>

Servo ColdServo[sizeof(ColdServoPins)];
uint32_t TServoActivated = 0;
uint32_t RFID_start_t = 0;

void servo_innit() {
	pinMode(SERVO_POWER_PIN, OUTPUT);
	for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
		ColdServo[i].attach(ColdServoPins[i]);
		
		char buff[60];
		sprintf(buff, "Arduino: Attaching servo %u on pin %u", i, ColdServoPins[i]);
		logInfo(buff);
	}
	for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
		ColdServo[i].write(SERVO_START_POS);
	}
	delay(500);
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
	}
}

void ejectColdDrink(const std_msgs::UInt8 &cmd_msg) {
	if (cmd_msg.data < sizeof(ColdServoPins)) {
		// Set time the servo was activated to set off reset after SERVO_TIMEOUT
		TServoActivated = static_cast<uint32_t>(millis());
		ColdServo[cmd_msg.data].write(SERVO_END_POS);
		char buff[60];
		sprintf(buff, "Arduino: Activating servo %u on pin %u", cmd_msg.data, ColdServoPins[cmd_msg.data]);
		logInfo(buff);
	}
}
