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

void updateProgress(uint8_t ring, uint16_t promille) {
	uint16_t progress = promilleToFrac(promille);
	bool reached_last = false;
	logInfo("Updating LED progress");
	
	// Loop and subtract 255 per pixel until we meet a fraction.
	// Set the pixel to the fraction and leave the other LED's off
	for (auto &pixel : led1) {
		if (reached_last) {
			pixel = 0;
		} else if (progress > 0xFF) {
			pixel = CRGB::White;
			progress -= 0xFF;
		} else {
			pixel = progress;
			reached_last = true;
		}
	}
}

void FastLED_show () {
	FastLED.show();
}
