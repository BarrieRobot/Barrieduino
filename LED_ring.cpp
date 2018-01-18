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

// TODO: use custom colour, see diaphragm mode function
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

inline uint16_t LED_ring::promilleToFrac(uint16_t promille) {
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

void LED_ring::diaphragmMode(bool direction) {
    // Get color that should be inserted in the array
    const CRGB color = CHSV(hue, sat, val);

    const uint16_t max_val (255 * 10);
    uint16_t subtractor = map(millis() - effectStart, 0, DIAPHRAGM_DURATION, 0, max_val);
    char buff[200];
    subtractor = (direction ? max_val - subtractor : subtractor);   // If the diaphragm is opening, the animation should be reversed
    sprintf(buff, "subtractor = %u", subtractor);

    // There are 6 blades in the diaphragm
    for (uint8_t i = 0; i < 6; ++i) {
        auto val = subtractor;
        bool reached_last = false;

        // There are 10 LEDs per blade of the diaphragm (60/6 blades)
        for (uint8_t j = 0; j < 10; ++j) {
            // uint8_t index = (i == 5 && j > DIAPHRAGM_LED_OFFSET ? j - DIAPHRAGM_LED_OFFSET : i*10 + j + DIAPHRAGM_LED_OFFSET);
            uint8_t index = i*10 + j + DIAPHRAGM_LED_OFFSET;
            if (index > NUM_LEDS) { index -= 60; }

            // Imagine this as a the 10 led moving with the diaphragm (---*######)
            // The LEDs that are supposed to be on up to the position of the blade at this moment
            if (reached_last)       // LED is on (#)
            {
                leds[index] = color;
            } else if (val > 0xFF)  // LED is off (-)
            {
                leds[index] = 0;
                val -= 0xFF;
            } else                  // LED is dimmed (*)
            {
                auto temp = color;
                leds[index] = temp.fadeToBlackBy(val);
                reached_last = true;
            }
        }
    }

    // After effect fade to black
    if (millis() - effectStart > DIAPHRAGM_DURATION) {
        setMode(0, 500);
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
            bool done = true;
            for (auto& led : leds) {
                led.fadeToBlackBy(30);  // Arbitrary value. Higher is faster fade.
                done &= !((bool) led);
            }
            reqUpdate = !done;  // Updates are only required if not fully faded yet
        } else if (mode == 1) {
            progressMode(param);
        } else if (mode == 2) {
            rainbowWave();
        } else if (mode == 3) {
            pulseMode(param);
        } else if (mode == 4) {
            pulseRainbow(param);
        } else if (mode == 5) {
            singlePulseMode(param);
        } else if (mode == 6) {
            diaphragmMode(static_cast<bool>(param));
        }
    }
}

void LED_ring::setMode(uint8_t _mode, uint16_t _param) {
    mode = _mode;
    param = _param;
    effectStart = millis();
    reqUpdate = true;
}
