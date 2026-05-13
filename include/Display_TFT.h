#ifndef DISPLAY_TFT_H
#define DISPLAY_TFT_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// ====== ĐỊNH NGHĨA CHÂN CẮM (Cổng HD4) ======
#define TFT_CS    41
#define TFT_DC    40
#define TFT_RST   45 
#define TFT_MOSI  47
#define TFT_SCK   21
#define TFT_MISO  -1  // Không dùng MISO để đọc từ màn hình lên terminal
#define TFT_BL    -1  // Mạch HD4 đã tự nối nguồn cho đèn nền nên để -1

// ====== KHAI BÁO CÁC HÀM ======
void display_Init();
void display_ShowTestMessage();
void display_ShowWaitCard();
void display_ShowUID(String ID);
void display_ShowStatus(String msg, uint16_t color);
void display_ClearText_Smart(String text, int x, int y, uint8_t textSize);
void display_ShowWeather(float temp, float hum);

// Tà ma ngoại đạo
void display_ShowMotionStatus(bool isDetected);

#endif