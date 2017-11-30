//
// Created by Casper on 30/11/2017.
//

#include "function_list.h"
#include "wire_scheme.h"
#include "LED_ring.h"

void LED_innit() {
	// Initialise FastLED
	FastLED.addLeds<LED_TYPE, LED1_PIN, COLOR_ORDER>(led1, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE, LED2_PIN, COLOR_ORDER>(led2, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(BRIGHTNESS);
	
	fill_solid(led1, NUM_LEDS, CRGB(60, 60, 60));
	FastLED.show();
	delay(500);
	fill_rainbow(led1, NUM_LEDS, 0, 7);
	FastLED.show();
	delay(500);
	fill_solid(led1, NUM_LEDS, CHSV(0, 0, 0));
	FastLED.show();
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
			led1[i] = 0;
		} else if (progress > 0xFF) {
			led1[i] = CRGB::White;
			progress -= 0xFF;
		} else {
			led1[i] = progress;
			reached_last = true;
		}
	}
}
