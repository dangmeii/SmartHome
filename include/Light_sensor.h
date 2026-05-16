#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <Arduino.h>

// Định nghĩa chân GPIO cho I2C (SDA, SCL)
#define BH1750_SDA 18
#define BH1750_SCL 17

void bh1750_Init();
float bh1750_ReadLux();

#endif