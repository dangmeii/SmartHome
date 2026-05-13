#include "Audio_system.h"
#include <LittleFS.h> // Gọi thư viện quản lý ổ cứng

Audio audio;

void audio_Init() {
    // 1. Đánh thức ổ cứng Flash
    if (!LittleFS.begin(true)) {
        // Lỗi không mở được bộ nhớ
        return; 
    }

    // 2. Khởi tạo cấu hình chân I2S
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DIN);
    audio.setVolume(5); // Max là 21
}

void audio_Loop() {
    audio.loop(); // Phải luôn chạy để bơm nhạc ra loa
}

void audio_PlayWarning() {
    // Nếu loa đang không kêu gì, thì bật file canhbao.mp3 lên
    if(!audio.isRunning()){
        audio.connecttoFS(LittleFS, "/Max-versteppen.wav"); 
    }
}