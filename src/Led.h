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
  Led() {}

  void setup()
  {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.setBrightness(brightness);
  }

  void update(uint32_t ms)
  {
    if (tickEffect.tick(ms))
    {
      effectManager.update(ms);
      FastLED.show();
    }
  }

  void updateData(LedData &data)
  {
    if (brightness != data.brightness)
    {
      brightness = data.brightness;
      // FastLED.setBrightness((brightness / 16) + 5);
       FastLED.setBrightness(brightness);
    }

    effectManager.updateData(data);
  }

  LedData getData()
  {
    LedEffect *effect = effectManager.getCurrentEffect();
    LedData data = {
        brightness,
        effectManager.getCurrentEffectNumber(),
        effect->getSpeed(),
        effect->getScale()};
        // effect->getName()};
    Serial.println("LED getData();");
    return data;
  }
};

#endif