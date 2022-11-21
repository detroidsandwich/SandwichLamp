#ifndef LED_H
#define LED_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <FastLED.h>
#include "LedEffect.h"
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

  void update()
  {
    uint32_t ms = millis();
    if (tickEffect.tick(ms))
    {
      effectManager.update(ms);
      FastLED.show();
    }

    // if (updateMode.tick(ms))
    // {
    //   effectManager.nextEffect();
    //   // effectManager.currentEffect->m_speed += 1;
    // }
  }

  void shiftEffectNumber(int8_t effectNumber)
  {
    effectManager.shiftEffectNumber(effectNumber);
  }

  LedEffect *currentEffect()
  {
    return effectManager.getCurrentEffect();
  };
};

#endif