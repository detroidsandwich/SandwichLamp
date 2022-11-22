#ifndef LED_H
#define LED_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <FastLED.h>
#include "LedSettings.h"
#include "Automat.h"
#include "EffectManager.h"

class Led
{
  Automat tickEffect{66};
  EffectManager effectManager;

public:
  Led() {}

  void setup()
  {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.setBrightness(5);
  }

  void update(uint32_t ms)
  {
    if (tickEffect.tick(ms))
    {
      effectManager.update(ms);
      FastLED.show();
    }
  }

  EffectManager &getEffectManager() { return effectManager; }
};

#endif