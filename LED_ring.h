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

CRGB led1[NUM_LEDS];
CRGB led2[NUM_LEDS];

#endif //COFFEE_LOADER_LED_RING_H
