//
// Created by Casper on 30/11/2017.
//

#include "ROS_code.h"



void ROS_init() {
	// Initialise ROS node and add subscribe to topics
	nh.initNode();
	nh.advertise(RFID_pub);
	nh.subscribe(LED_subscriber);
	nh.subscribe(ColdDrink_subscriber);
	
	//nh.serviceClient(client);
	//nh.advertiseService(server);
}
