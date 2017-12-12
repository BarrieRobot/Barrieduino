#include <Arduino.h>

#include "wire_scheme.h"
#include "function_list.h"
#include "ROS_code.h"

/*
 * Rules of using ROS in Arduino code:
 *
 * Rule 1: No shared objects in header files. NONE, DO YOU HEAR ME?!@
 * Rule 2: All ROS things go in the main file (this one).
 *         Transferring it to a file which does nog contain setup() and loop() will not work.
 * Rule 3: Everything that you want to influence from within this file (or another) you do using functions.
 *         No direct mingling, USE GETTERS AND SETTERS YOU FUCK
 *
 * Thank you for your attention.
 */


/*/-- Definitions --/*/

#define SERVO_START_POS 0
#define SERVO_END_POS   90
#define SERVO_TIMEOUT   1000

#define RFID_TIMEOUT    50


/*/-- Function declarations --/*/

// See function_list.h

/*/-- Object declarations --/*/

// Sensors
#include "Sensors.h"
Sensors sensors;

// Servos
// See servo_code.cpp

#include "CoffeeMachine.h"
CoffeeMachine coffeeMachine;

uint32_t RFID_start_t = 0;

/*/-- Code --/*/

void ROS_init() {
    // Initialise ROS node and add subscribe to topics
    nh.initNode();
    nh.advertise(RFID_pub);
    nh.subscribe(LED_subscriber);
    nh.subscribe(ColdDrink_subscriber);

    //nh.serviceClient(client);
    //nh.advertiseService(server);
}

void logInfo(char* message) { nh.loginfo(message); }
void logWarn(char* message) { nh.logwarn(message); }

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

    // Initialise all sensors
    sensors.begin();

    coffeeMachine.begin();

    // Initialise ROS, its subscribers, publishers and services
    ROS_init();
    
    // Initialise all servos
    servo_innit();
    
    //while(!nh.connected()) nh.spinOnce();
    logInfo("Arduino: Startup complete");
}

void loop() {
    // Send data to LED strip
    FastLED_show();
    
    // If a new tag is detected, wait RFID_TIMEOUT ms and collect the tag info
    if (Serial1.available()) {
        if (!RFID_start_t) {
            RFID_start_t = static_cast<uint32_t>(millis());
        } else if (millis() > RFID_start_t + RFID_TIMEOUT) {
            uint32_t id = getTagInfo();
            if (id) {
                RFID_msg.data = id;
                RFID_pub.publish(&RFID_msg);
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

    // Update coffee machine status
    coffeeMachine.update();
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

/*void callback(const Test::Request &req, Test::Response &res) {
    if ((j++) % 2)
        res.output = "hello";
    else
        res.output = "world";
}*/
