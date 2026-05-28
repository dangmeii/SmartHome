#include "DHT.h"
#include "Display_TFT.h"
#define DHTPIN 9    // Chân DATA nối vào IO9
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void weather_Init() {
    dht.begin();
}

// Hàm này sẽ được gọi mỗi 2 giây
void weather_Update(float &t, float &h) {
     h = dht.readHumidity();
    t = dht.readTemperature();

    // Kiểm tra xem cảm biến có bị lỗi không
    if (isnan(h) || isnan(t)) {
        Serial.println("Loi doc DHT11!");
        return;
    }

    // Đẩy dữ liệu ra màn hình TFT
    display_ShowWeather(t, h);
    
    // Log ra Serial để debug
    Serial.printf("Temp: %.1f, Hum: %.1f\n", t, h);
}