#ifndef LED_H
#define LED_H

#include <Esp.h>
#include <FastLED.h>
#include "LedSettings.h"
#include "Automat.h"
#include "EffectManager.h"
#include "LedData.h"

class Led
{
  Automat tickEffect{16};
  EffectManager effectManager;
  byte brightness = 255; // todo

public:
  Led(){};

  void setup(LedData *data)
  {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

    updateBrightness(data->brightness);

    effectManager.setup(data);
  }

  void updateData(LedData *data)
  {
    if (brightness != data->brightness)
    {
      updateBrightness(data->brightness);
    }

    effectManager.updateData(data);
  }

  void update(uint32_t ms)
  {
    if (tickEffect.tick(ms))
    {
      effectManager.update(ms);
      FastLED.show();
    }
  }

  void updateBrightness(byte newBrigthness)
  {
    brightness = newBrigthness;
    FastLED.setBrightness((brightness / 9) + 2);
  }
};

#endif