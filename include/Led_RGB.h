#ifndef LED_RGB_H
#define LED_RGB_H

#include <Adafruit_NeoPixel.h>

#define RGB_PIN 48
#define NUM_PIXELS 1

void rgb_Init();
void rgb_SetColor(uint8_t r, uint8_t g, uint8_t b);

#endif