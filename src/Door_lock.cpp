#include "Door_lock.h"
// ====== CẤU HÌNH HARDWARE TIMER (LEDC) ======
const int pwmFreq = 50;       // Tần số 50Hz chuẩn cho điều khiển Servo
const int pwmResolution = 10; // Độ phân giải 10-bit (giá trị PWM chạy từ 0 đến 1023)
const int pwmChannel = 4;     // Ép dùng Kênh số 4 (để né xung đột với màn hình TFT)

// ---DÙNG VỚI SERVO 360 ĐỘ ---
const int STOP_SERVO = 77;   // Điểm phanh đứng im
const int PULL_LATCH = 100;  // Quay thu chốt vào
const int PUSH_LATCH = 40;   // Quay đẩy chốt ra

void door_Init() {
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(SERVO_PIN, pwmChannel);
    
    // Vừa bật lên là phải ĐỨNG IM chờ lệnh
    ledcWrite(pwmChannel, STOP_SERVO); 
}

void door_Unlock() {
    // 1. KÉO CHỐT RA (Quay để mở cửa)
    ledcWrite(pwmChannel, PULL_LATCH); 
    delay(800); // Cho nó quay khoảng 0.8 giây để rút đủ sâu (Bạn tự tăng giảm số này cho vừa cửa)
    
    // 2. PHANH LẠI! Giữ cửa mở
    ledcWrite(pwmChannel, STOP_SERVO);
    
    // 3. Chờ người đi qua
    delay(5000); 
    
    // 4. ĐẨY CHỐT VÀO (Quay ngược lại để khóa)
    ledcWrite(pwmChannel, PUSH_LATCH);
    delay(600); // Thời gian đẩy phải BẰNG thời gian rút (0.8 giây)
    
    // 5. PHANH LẠI! Trạng thái khóa cửa an toàn
    ledcWrite(pwmChannel, STOP_SERVO);
}