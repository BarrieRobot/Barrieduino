#include "function_list.h"
#include "wire_scheme.h"
#include "LED_ring.h"

void LED_ring::begin(const uint8_t ring) {
    // Initialise FastLED
    if (ring == 0) {
        FastLED.addLeds<LED_TYPE, LED1_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    } else {
        FastLED.addLeds<LED_TYPE, LED2_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    }
    FastLED.setBrightness(BRIGHTNESS);
}

void LED_ring::progressMode(uint16_t promille) {
    uint16_t progress = promilleToFrac(promille);
    bool reached_last = false;
    logInfo("Updating LED progress");

    // Loop and subtract 255 per pixel until we meet a fraction.
    // Set the pixel to the fraction and leave the other LED's off
    for (auto &pixel : leds) {
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

uint16_t LED_ring::promilleToFrac(uint16_t promille) {
    return (((uint32_t) promille << 8) * (uint8_t) NUM_LEDS) / 1000;
}

void LED_ring::setBaseColor(uint8_t _hue, uint8_t _sat, uint8_t _val) {
    hue = _hue;
    sat = _sat;
    val = _val;
}

void LED_ring::show() {
    FastLED.show();
}

void LED_ring::setMode(uint8_t _mode) {
    mode = _mode;
}
