#ifndef LED_DATA_H
#define LED_DATA_H

#include <Esp.h>

struct LedData
{
    byte brightness;
    byte numberEffect;
    byte speed;
    byte scale;
};

#endif