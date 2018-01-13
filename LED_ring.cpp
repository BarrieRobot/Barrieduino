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

// TODO: use custom colour
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
    reqUpdate = false;  // No updates are required as long as the parameter doesn't change
}

uint16_t LED_ring::promilleToFrac(uint16_t promille) {
    return (((uint32_t) promille << 8) * (uint8_t) NUM_LEDS) / 1000;
}

void LED_ring::rainbowWave() {
    fill_rainbow(leds, NUM_LEDS, hue, 1);
    hue++;
}

void LED_ring::pulseMode(uint16_t duration) {
    // Set the colour to the one defined by the host
    CRGB colour = CHSV(hue, sat, val);
    // Get brightness value
    uint8_t mapVal = abs(map(millis() - effectStart, 0, duration, -255, 255));
    // Use FastLED to dim colour
    colour %= mapVal;

    // Set all pixels to the calculated value
    for (auto& pixel : leds) {
        pixel = colour;
    }
    // Loop effect if is has completed the cycle
    if (millis() - effectStart > duration) {
        effectStart = millis();
    }
}

void LED_ring::pulseRainbow(uint16_t duration) {
    auto old = effectStart;
    pulseMode(duration);
    if (effectStart != old) {
        hue += 75;  // Arbitrary value, just has to change the colour a bit
    }
}

void LED_ring::singlePulseMode(uint16_t duration) {
    auto old = effectStart;
    pulseMode(duration);
    if (effectStart != old) {
        reqUpdate = false;
    }
}

void LED_ring::setBaseColor(uint8_t _hue, uint8_t _sat, uint8_t _val) {
    hue = _hue;
    sat = _sat;
    val = _val;
}

void LED_ring::show() {
    // Only calculate an update when one is actually necessary
    if (reqUpdate) {
        if (mode == 0) {
            progressMode(param);
        } else if (mode == 1) {
            rainbowWave();
        } else if (mode == 2) {
            pulseMode(param);
        } else if (mode == 3) {
            pulseRainbow(param);
        } else if (mode == 4) {
            singlePulseMode(param);
        }
    }
}

void LED_ring::setMode(uint8_t _mode, uint16_t _param) {
    mode = _mode;
    param = _param;
    effectStart = millis();
    reqUpdate = true;
}
