#include <Arduino.h>

#include "DallasTemperature.h"
#include <HX711_ADC.h>

#include "wire_scheme.h"
#include "function_list.h"
#include "ROS_code.h"


/*/-- Definitions --/*/

#define SERVO_START_POS 0
#define SERVO_END_POS   90
#define SERVO_TIMEOUT   1000

#define RFID_TIMEOUT    50


/*/-- Function declarations --/*/

// See function_list.h

/*/-- Object declarations --/*/

// FastLED
#include "LED_ring.h"

// Sensors
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

// Servos
// See servo_code.cpp

uint32_t RFID_start_t = 0;

/*/-- Code --/*/

void setup() {
	delay(500);
	
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	
	// Initiate USB serial connection used for ROS
	Serial.begin(57600);
	// Initiate RFID reader connection
	Serial1.begin(19200);
	
	// Initialise FastLED rings
	LED_innit();
	
	sensors.begin();
	
	// Initialise ROS, its subscribers, publishers and services
	ROS_init();
	
	// Initialise all servos
	servo_innit();
	
	//while(!nh.connected()) nh.spinOnce();
	nh.loginfo("Arduino: Startup complete");
}

void loop() {
	// Send data to LED strip
	FastLED.show();
	//FastLED.delay(1000/FPS);
	
	/*sensors.requestTemperatures();
	sensors.getTempCByIndex(0);*/
	
	// If a new tag is detected, wait RFID_TIMEOUT ms and collect the tag info
	if (Serial1.available()) {
		if (!RFID_start_t) {
			RFID_start_t = static_cast<uint32_t>(millis());
		} else if (millis() > RFID_start_t + RFID_TIMEOUT) {
			uint32_t id = getTagInfo();
			if (id) {
				//digitalWrite(LED_BUILTIN, HIGH);
				RFID_msg.data = id;
				RFID_pub.publish(&RFID_msg);
				//delay(500);
				//digitalWrite(LED_BUILTIN, LOW);
			}
		}
	}
	
	// Do all necessary ROS synchronisations (at least once per 5 seconds)
	nh.spinOnce();
	
	digitalWrite(LED_BUILTIN, HIGH);
	delay(10);
	digitalWrite(LED_BUILTIN, LOW);
	
	// See if servos might need to be reset to closed position
	update_servos();
}

uint32_t getTagInfo() {
	RFID_start_t = 0;
	uint32_t id = 0;
	uint8_t i = 0;
	while (Serial1.available()) {
		id = (id << 8) + (uint8_t) Serial1.read();
		i++;
	}
	// A tag returns an id of 4 bytes length. A double read should thus be discarded
	if (i != 4) { return 0; }
	else { return id; }
}

int j = 0;

void callback(const Test::Request &req, Test::Response &res) {
	if ((j++) % 2)
		res.output = "hello";
	else
		res.output = "world";
}
