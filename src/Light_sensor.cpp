#include <Wire.h>
#include <BH1750.h>

// Khởi tạo đối tượng cảm biến
BH1750 lightMeter;

// Khai báo chân GPIO cho I2C (SDA, SCL)
#define BH1750_SDA 18 
#define BH1750_SCL 17  

void bh1750_Init() {
    // Ép ESP32 phải mở chuẩn I2C ở đúng 2 chân này
    Wire.begin(BH1750_SDA, BH1750_SCL); 

    // Bật cảm biến (Chế độ đọc liên tục, độ phân giải cao)
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("✅ Khoi tao BH1750 THANH CONG!");
    } else {
        Serial.println("❌ LOI: Khong the ket noi BH1750, kiem tra lai day/chan I2C!");
    }
}

// Hàm đọc giá trị ánh sáng (lux) từ cảm biến
float bh1750_ReadLux() {
    return lightMeter.readLightLevel();
}