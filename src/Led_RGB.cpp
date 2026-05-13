#include "Led_RGB.h"

Adafruit_NeoPixel pixels(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void rgb_Init() {
    pixels.begin();
    pixels.setBrightness(50); // Độ sáng 50% cho đỡ chói mắt
    pixels.show(); // Tắt LED lúc khởi tạo
}

void rgb_SetColor(uint8_t r, uint8_t g, uint8_t b) {
    pixels.setPixelColor(0, pixels.Color(r, g, b));
    pixels.show();
}