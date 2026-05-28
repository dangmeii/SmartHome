#define BLYNK_TEMPLATE_ID "TMPL6ZHzmcct4"
#define BLYNK_TEMPLATE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "OKFsbFnbq7O8jVdhc86ieIT6Iulath9H"

#include <WiFiManager.h> // Thư viện quản lý WiFi 
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>

  // thư viện tự định nghĩa
#include "Display_TFT.h" 
#include "Handle_RFID.h"
#include "Door_lock.h"
#include "Weather.h"
#include "Motion.h"
#include "Led_RGB.h"
#include "Audio_system.h"
#include "Light_sensor.h"
#include "MQ2_sensor.h" 
#include "Rain_sensor.h"

// Định nghĩa chân LED trên board
#define ONBOARD_LED 38
// ====== KHAI BÁO MÃ THẺ CHỦ (MASTER CARD) ======
const String MASTER_ID = "91 D9 F2 06";

// ===== BIẾN THỜI GIAN CHO DHT11 =====
// Bắt buộc dùng kiểu unsigned long vì số millis() sẽ rất to
unsigned long previousDHTMillis = 0;
const long dhtInterval = 2000; // Khoảng thời gian giữa 2 lần đọc (s)

// ===== Variable của module Weather =====
float nhietDo = 0;
float doAm = 0;
int mucDoMua = rain_Read(); 

// ===== variable của module Motion =====
static bool trangThaiPIR_Cu = false;
static unsigned long thoiDiemBatXanh = 0;
static bool dangODoTreXanh = false;
int SolanMoCua = 0; 


// ===== variable của module Gas =====
    static unsigned long thoiDiemCapNhatTFT_Gas = 0;
    unsigned long thoiGianHienTai = millis();
    int nongDoGas = mq2_ReadGas();

// ===== Variable bắn lên Bllnk =====
    unsigned long thoiDiemCapNhatBlynk = 0;

void setupWiFi() {
    WiFiManager wifiManager;
    wifiManager.autoConnect("ESP32-Access-Point");
}

void setup() {
    // Khởi tạo Serial 
    Serial.begin(115200);
    // delay(2000);
    setupWiFi();

    // Đánh thức Blynk
    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect();

    // Hàm khởi tạo các module (để run các bus riêng biệt)
    display_Init();
    rfid_Init();
    door_Init();
    weather_Init();
    motion_Init();
    rgb_Init();
    audio_Init();
    bh1750_Init();  
    mq2_Init();  

    // Cài đặt chế độ LED
    pinMode(ONBOARD_LED, OUTPUT);   
    rgb_SetColor(0, 0, 0); // tắt đèn lúc đầu

    // Hàm in chữ + chờ quét thẻ
    display_ShowTestMessage();
    delay(1000);
    display_ClearText_Smart("NHOM 3 DEP TRAI", 20, 60, 2);
    display_ShowWaitCard();
    display_ShowMotionStatus(motion_Detected());

    // Hàm nồng độ
    display_ShowGasLevel(mq2_ReadGas());
    
}

void loop() {
    Blynk.run();

    // LẤY THỜI GIAN THỰC CHO TOÀN BỘ HỆ THỐNG DÙNG CHUNG
    unsigned long currentMillis = millis();

    // NHIỆM VỤ 1: CẬP NHẬT THỜI TIẾT VÀ HIỂN THỊ TRÊN TFT
    if (currentMillis - previousDHTMillis >= dhtInterval) {
        previousDHTMillis = currentMillis;
        weather_Update(nhietDo, doAm); 
    }

    static unsigned long thoiDiemCapNhatTFT_Rain = 0;
    if (currentMillis - thoiDiemCapNhatTFT_Rain >= 3000) {
        display_rainLevel(mucDoMua);
        thoiDiemCapNhatTFT_Rain = currentMillis;
    }

    // NHIỆM VỤ 2: QUẸT THẺ RFID
    // Lưu giá trị ID của thẻ vào memory
    String cardID = rfid_GetUID();
    
    // Nếu có thẻ được quét
    if (cardID != "") {

        display_ShowUID(cardID);

        delay(1000); // Tạm dừng 1s để đọc kịp chữ

    // ====== LOGIC KIỂM TRA THẺ ======

    if (cardID == MASTER_ID) {
            // NẾU THẺ ĐÚNG
            display_ShowStatus("MO CUA...", ST77XX_GREEN);
            // GỌI HÀM MỞ CỬA 
            door_Unlock(); 

            display_ClearText_Smart("MO CUA...", 20, 180, 2);

            SolanMoCua++; 
            
        } else {
            // NẾU THẺ SAI
            display_ShowStatus("SAI THE!", ST77XX_RED);
            delay(2000); // Treo màn hình 2 giây cho người ta biết là bị từ chối

            display_ClearText_Smart("SAI THE!", 20, 180, 2);
        }
        
        // Sau khi xử lý xong (mở cửa xong hoặc cảnh báo xong)
        display_ClearText_Smart("ID: " + cardID, 20, 150, 2);

        // Reset lại màn hình về trạng thái chờ 
        display_ShowWaitCard(); 
    }  

     // NHIỆM VỤ 3: CẬP NHẬT NỒNG ĐỘ GAS MỖI 1.5 GIÂY
    if (thoiGianHienTai - thoiDiemCapNhatTFT_Gas >= 1500) {
        display_ShowGasLevel(nongDoGas); 
        thoiDiemCapNhatTFT_Gas = thoiGianHienTai; // Reset bộ đếm
    }

    // NHIỆM VỤ 4: THEO DÕI CHUYỂN ĐỘNG (In thẳng lên TFT)
   
    // 1. Đọc dữ liệu từ sensor
    bool coNguoi = motion_Detected();
    float doSang = bh1750_ReadLux();

    // 2. Sự kiện vừa mới rời đi 
   if (coNguoi != trangThaiPIR_Cu) {
        
        display_ShowMotionStatus(coNguoi); // Chớp TFT
        
        if (coNguoi) {
            // thời điểm không độ trễ 
            dangODoTreXanh = false; // Có người -> Hủy ngay màu xanh
        } 
        else {
            dangODoTreXanh = true;             // Người vừa đi -> Kích hoạt xanh
            thoiDiemBatXanh = thoiGianHienTai; // Bắt đầu đếm 5s
        }
        
        // Cập nhật lại memory 
        trangThaiPIR_Cu = coNguoi; 
    }

    // 3. Khối hiển thị LED 
    // 0: Tắt | 1: Đỏ | 2: Xanh | 3: Trắng | 4: Tím (Báo cháy)

    static int mauLedHienTai = -1; 
    int mauCanBat = 0; 

    // Ngưỡng báo động 
    int NGUONG_BAO_CHAY = 2000; 

    // A. XÁC ĐỊNH MÀU CẦN BẬT DỰA VÀO LOGIC ƯU TIÊN
    if (nongDoGas > NGUONG_BAO_CHAY) {
        mauCanBat = 4; // ƯU TIÊN 1: BÁO CHÁY! 
    }
    else if (coNguoi) {
        // màu cần bật khi có người: ưu tiên ĐỎ > XANH > TRẮNG

        mauCanBat = 1; // Ưu tiên 2: Đỏ báo động
    } 
    else if (dangODoTreXanh) {
        mauCanBat = 2; // Ưu tiên 3: Xanh an toàn
        
        // Kiểm tra xem đã ôm màu xanh đủ 5000ms chưa?
        if (thoiGianHienTai - thoiDiemBatXanh >= 5000) {
            dangODoTreXanh = false; 
        }
    }

    else {
        // Ưu tiên 4: Cảm biến ánh sáng
        
        // BỘ LỌC NHIỄU: Phải >= 0 mới tin, nếu trả về số âm là do nhiễu I2C -> Bỏ qua
        if (doSang >= 0.0) { 
            if (doSang < 20.0) { 
                mauCanBat = 3; // Trời tối -> Cần bật Trắng
            } else {
                mauCanBat = 0; // Trời sáng -> Cần Tắt
            }
        } else {
            // Nếu đọc lỗi (doSang < 0), giữ nguyên màu cũ, không làm gì cả
            mauCanBat = mauLedHienTai; 
        }
    }

    // B. CHỈ RA LỆNH CHO IC KHI MÀU BỊ THAY ĐỔI (TUYỆT CHIÊU CHỐNG NHÁY)
    if (mauCanBat != mauLedHienTai) {
        if (mauCanBat == 4)      rgb_SetColor(255, 0, 255);   // Tím (Báo cháy)
        else if (mauCanBat == 1) rgb_SetColor(255, 0, 0);     // Đỏ
        else if (mauCanBat == 2) rgb_SetColor(0, 255, 0);     // Xanh
        else if (mauCanBat == 3) rgb_SetColor(255, 255, 255); // Trắng
        else if (mauCanBat == 0) rgb_SetColor(0, 0, 0);       // Tắt hoàn toàn
        
        // Cập nhật lại màu hiện tại
        mauLedHienTai = mauCanBat;
    }

    // NHIỆM VỤ 5: BẮN DỮ LIỆU LÊN BLYNK (Nhiệt độ, độ ẩm, mưa, gas)
    if (currentMillis - thoiDiemCapNhatBlynk >= 3000){
        weather_Update(nhietDo, doAm); // Cập nhật lại nhiệt độ và độ ẩm mới nhất
        mucDoMua = rain_Read();
        nongDoGas = mq2_ReadGas();
    
            thoiDiemCapNhatBlynk = currentMillis; // Reset bộ đếm
    
            // Bắn dữ liệu lên Blynk
        Blynk.virtualWrite(V0, SolanMoCua);
        Blynk.virtualWrite(V1, nhietDo); 
        Blynk.virtualWrite(V2, doAm);    
        Blynk.virtualWrite(V3, nongDoGas);
        Blynk.virtualWrite(V4, mucDoMua);
    }
}

