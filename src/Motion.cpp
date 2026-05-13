#include "Motion.h"

void motion_Init() {
    pinMode(PIR_PIN, INPUT);
}

// Thằng này chỉ đi tuần tra, hễ thấy người là hô lên "CÓ!" (return true)
bool motion_Detected() {
    // int state = digitalRead(PIR_PIN);
    // if (state == HIGH) {
    //     return true;  // Có chuyển động
    // } else {
    //     return false; // Không có ai
    // }
    return (digitalRead(PIR_PIN) == HIGH);
}