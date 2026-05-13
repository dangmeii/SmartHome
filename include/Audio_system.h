#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "Audio.h"

// Khai báo chân I2S
#define I2S_LRC    16
#define I2S_BCLK   15
#define I2S_DIN     7

void audio_Init();
void audio_Loop();
void audio_PlayWarning();

#endif