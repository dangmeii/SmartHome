#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

void weather_Init();
void weather_Update(float &t, float &h);

#endif