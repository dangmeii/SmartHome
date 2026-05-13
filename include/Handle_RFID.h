#ifndef RFID_HANDLER_H
#define RFID_HANDLER_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// ====== ĐỊNH NGHĨA CHÂN CẮM (Cổng HD11) ======
#define RFID_CS_PIN    48
#define RFID_SCK_PIN   19
#define RFID_MOSI_PIN  35
#define RFID_MISO_PIN  20
#define RFID_RST_PIN   0

// ====== KHAI BÁO CÁC HÀM ======
void rfid_Init();
String rfid_GetUID();

#endif