#include <Arduino.h>
  // thư viện tự định nghĩa
#include "Display_TFT.h" 
#include "Handle_RFID.h"
#include "Door_lock.h"
#include "Weather.h"
#include "Motion.h"
#include "Led_RGB.h"
#include "Audio_system.h"

// Định nghĩa chân LED trên board
#define ONBOARD_LED 38
// ====== KHAI BÁO MÃ THẺ CHỦ (MASTER CARD) ======
const String MASTER_ID = "91 D9 F2 06";

// ===== BIẾN THỜI GIAN CHO DHT11 =====
// Bắt buộc dùng kiểu unsigned long vì số millis() sẽ rất to
unsigned long previousDHTMillis = 0;
const long dhtInterval = 2000; // Khoảng thời gian giữa 2 lần đọc (s)

// Variable của module
float nhietDo = 0;
float doAm = 0;

void setup() {
    // Khởi tạo Serial 
    Serial.begin(115200);
    // delay(2000);

    // Hàm khởi tạo các module (để run các bus riêng biệt)
    display_Init();
    rfid_Init();
    door_Init();
    weather_Init();
    motion_Init();
    rgb_Init();
    audio_Init();
   
    // Cài đặt chế độ LED
    pinMode(ONBOARD_LED, OUTPUT);   
    rgb_SetColor(0, 0, 0); // tắt đèn lúc đầu

    // Hàm in chữ + chờ quét thẻ
    display_ShowTestMessage();
    delay(1000);
    display_ClearText_Smart("TOP 1 SV VN", 20, 60, 3);
    display_ShowWaitCard();
    display_ShowMotionStatus(motion_Detected());
}

void loop() {
    // LẤY THỜI GIAN THỰC CHO TOÀN BỘ HỆ THỐNG DÙNG CHUNG
    unsigned long currentMillis = millis();

    // NHIỆM VỤ 1: CẬP NHẬT THỜI TIẾT
    if (currentMillis - previousDHTMillis >= dhtInterval) {
        previousDHTMillis = currentMillis;
        weather_Update();
    }

    // NHIỆM VỤ 2: QUẸT THẺ RFID
    // Lưu giá trị ID của thẻ vào memory
    String cardID = rfid_GetUID();
    
    // Nếu có thẻ được quét
    if (cardID != "") {

// trả lại 
        // Serial.println("UID: " + cardID);

        display_ShowUID(cardID);

    // Kêu bíp bíp (Nếu mạch có còi) hoặc chớp đèn rơ le ở đây
        delay(1000); // Tạm dừng 1s để đọc kịp chữ

    // ====== LOGIC KIỂM TRA THẺ ======

    if (cardID == MASTER_ID) {
            // NẾU THẺ ĐÚNG
            display_ShowStatus("MO CUA...", ST77XX_GREEN);
            // GỌI HÀM MỞ CỬA 
            door_Unlock(); 

            display_ClearText_Smart("MO CUA...", 20, 210, 2);
            
        } else {
            // NẾU THẺ SAI
            display_ShowStatus("SAI THE!", ST77XX_RED);
            delay(2000); // Treo màn hình 2 giây cho người ta biết là bị từ chối

            display_ClearText_Smart("SAI THE!", 20, 210, 2);
        }
        
        // Sau khi xử lý xong (mở cửa xong hoặc cảnh báo xong)
        display_ClearText_Smart("ID: " + cardID, 20, 150, 2);

        // Reset lại màn hình về trạng thái chờ 
        display_ShowWaitCard(); 
    }  

    // NHIỆM VỤ 3: THEO DÕI CHUYỂN ĐỘNG (In thẳng lên TFT)
    // ==========================================
    audio_Loop();

    static bool tinhTrangCu = false;
    bool coNguoi = motion_Detected();

    if (coNguoi != tinhTrangCu) {
        // Gọi hàm hiển thị bên module màn hình
        display_ShowMotionStatus(coNguoi); 
        
        if (coNguoi) {
            rgb_SetColor(255, 0, 0); // Sáng màu ĐỎ khi có người
        
            // audio_PlayWarning();

        } else {
            rgb_SetColor(0, 255, 0); // Sáng màu XANH LÁ khi an toàn
        }
        rgb_SetColor(0,0,0);
        tinhTrangCu = coNguoi; // Ghi nhớ lại trạng thái
    }
}

