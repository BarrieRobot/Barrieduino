#include <Arduino.h>

#include <Servo.h>
#include "FastLED.h"

#include "ros.h"
#include <std_msgs/UInt8.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/String.h>
#include <rosserial_arduino/Test.h>
using rosserial_arduino::Test;

#include "DallasTemperature.h"
#include <HX711_ADC.h>

FASTLED_USING_NAMESPACE


/*/-- Definitions --/*/

#define LED_DATA_PIN 	31
#define LED_TYPE 		WS2812B
#define COLOR_ORDER 	GRB
#define NUM_LEDS 		64

#define BRIGHTNESS 		80
#define FPS				120

#define ONE_WIRE_PIN 	2

#define ENABLE_PIN  	3
#define SERVO_START_POS 0
#define SERVO_END_POS 	90
#define SERVO_TIMEOUT 	1000

#define RFID_TIMEOUT 	50


/*/-- Function declarations --/*/

inline uint16_t promilleToFrac(uint16_t promille);
void updateProgress(const std_msgs::UInt16& cmd_msg);
void ejectColdDrink(const std_msgs::UInt8 &cmd_msg);
uint32_t getTagInfo();
void callback(const Test::Request & req, Test::Response & res);


/*/-- Object declarations --/*/

// FastLED
CRGB leds[NUM_LEDS];

// Robot Operating System
ros::NodeHandle	nh;

//ros::ServiceClient<Test::Request, Test::Response> client("test_srv");
//ros::ServiceServer<Test::Request, Test::Response> server("test_srv", &callback);

ros::Subscriber<std_msgs::UInt16> LED_subscriber("LED_progress", &updateProgress);
ros::Subscriber<std_msgs::UInt8> ColdDrink_subscriber("cold_drinks", &ejectColdDrink);

std_msgs::UInt32 RFID_msg;
ros::Publisher RFID_pub("RFID", &RFID_msg);

// Sensors
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

// Servos
uint8_t ColdServoPins[] = {10, 6, 7, 8};
Servo ColdServo[sizeof(ColdServoPins)];
uint32_t TServoActivated = 0;
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
	
	// Initialise FastLED
	FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(BRIGHTNESS);
  
  sensors.begin();

  fill_solid(leds, NUM_LEDS, CRGB(60, 60, 60));
  FastLED.show();
  delay(500);
  fill_rainbow(leds, NUM_LEDS, 0, 7);
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CHSV(0,0,0));
  FastLED.show();
  
  
	// Initialise ROS node and add subscribe to topics
	nh.initNode();
  nh.advertise(RFID_pub);
	nh.subscribe(LED_subscriber);
	nh.subscribe(ColdDrink_subscriber);
	
	//nh.serviceClient(client);
	//nh.advertiseService(server);
  
  // Initialise all servos
  pinMode(ENABLE_PIN, OUTPUT);
  for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
    ColdServo[i].attach(ColdServoPins[i]);
    
    char buff[60];
    sprintf(buff, "Arduino: Attaching servo %u on pin %u", i, ColdServoPins[i]);
    nh.loginfo(buff);
  }
  for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
    ColdServo[i].write(SERVO_START_POS);
  }
  delay(500);
  digitalWrite(ENABLE_PIN, LOW);  // The relay is inverted
	
	//while(!nh.connected()) nh.spinOnce();
	nh.loginfo("Arduino: Startup complete");
}

void loop() {
	// Send data to LED strip
	FastLED.show();
	
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
	
	nh.spinOnce();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
	
	// Reset servo position after SERVO_TIMEOUT milliseconds
	// to allow for the next cold drink to take position
	if (TServoActivated && (millis() > TServoActivated + SERVO_TIMEOUT)) {
		for (uint8_t i = 0; i < sizeof(ColdServoPins); ++i) {
			ColdServo[i].write(SERVO_START_POS);
		}
		TServoActivated = 0;
	}
	
	//FastLED.delay(1000/FPS);
}

uint16_t promilleToFrac(uint16_t promille) {
	return (((uint32_t) promille << 8) * (uint8_t) NUM_LEDS) / 1000;
}

void updateProgress(const std_msgs::UInt16 &cmd_msg) {
	uint16_t progress = promilleToFrac(cmd_msg.data);
	bool reached_last = false;
	
	// Loop and subtract 255 per pixel until we meet a fraction.
	// Set the pixel to the fraction and leave the other LED's off
	for (uint8_t i = 0; i < NUM_LEDS; ++i) {
		if (reached_last) {
			leds[i] = 0;
		} else if (progress > 0xFF) {
			leds[i] = CRGB::White;
			progress -= 0xFF;
		} else {
			leds[i] = progress;
			reached_last = true;
		}
	}
}

void ejectColdDrink(const std_msgs::UInt8 &cmd_msg) {
	if (cmd_msg.data < sizeof(ColdServoPins)) {
		// Set time the servo was activated to set off reset after SERVO_TIMEOUT
		TServoActivated = static_cast<uint32_t>(millis());
		ColdServo[cmd_msg.data].write(SERVO_END_POS);
    char buff[60];
    sprintf(buff, "Arduino: Activating servo %u on pin %u", cmd_msg.data, ColdServoPins[cmd_msg.data]);
    nh.loginfo(buff);
	}
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
	if (i != 4) { return  0; }
	else { return id; }
}

int j = 0;

void callback(const Test::Request &req, Test::Response &res) {
	if((j++)%2)
		res.output = "hello";
	else
		res.output = "world";
}
