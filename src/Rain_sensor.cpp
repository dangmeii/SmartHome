#include "Rain_sensor.h"

#define RAIN_PIN 7 

int rain_Read() {
    return analogRead(RAIN_PIN);
}