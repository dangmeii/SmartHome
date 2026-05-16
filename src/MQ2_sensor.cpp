#include "MQ2_sensor.h"

#define MQ2_PIN 8 

void mq2_Init() {
    // Không cần cấu hình pinMode cho hàm analogRead trên ESP32

    Serial.println("✅ Khoi tao MQ2 THANH CONG!");
}

int mq2_ReadGas() {
    // Đọc điện áp và quy đổi ra số từ 0 - 4095
    return analogRead(MQ2_PIN); 
}