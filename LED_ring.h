//
// Created by Casper on 30/11/2017.
//

#ifndef COFFEE_LOADER_LED_RING_H
#define COFFEE_LOADER_LED_RING_H

#include <FastLED.h>

#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define NUM_LEDS        64

#define BRIGHTNESS      80
#define FPS             120

class LED_ring {
public:
    void begin(uint8_t ring);
    void setMode(uint8_t mode);
    void progressMode(uint16_t promille);
    void setBaseColor(uint8_t hue, uint8_t sat, uint8_t val);
    void show();

private:
    inline uint16_t promilleToFrac(uint16_t promille);
    CRGB leds[NUM_LEDS];
    uint8_t hue = 0, sat = 0, val = 255;
    uint8_t mode = 0;
};

#endif //COFFEE_LOADER_LED_RING_H
