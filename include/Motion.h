#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>
#define PIR_PIN 36 //Khai báo chân IO module HW-416A

void motion_Init();
bool motion_Detected(); // Hàm này trả về đúng (true) hoặc sai (false)

#endif