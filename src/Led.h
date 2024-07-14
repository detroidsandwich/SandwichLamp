#ifndef LED_H
#define LED_H

#include <Esp.h>
#include <FastLED.h>
#include "LedSettings.h"
#include "Automat.h"
#include "Counter.h"

class Led
{
  byte brightness = 255; // todo
  bool lightMode = false;
  Counter _counter{};

public:
  Led(){};

  void setup()
  {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.setBrightness(brightness);
  }

  void update(){
     FastLED.show();
  }

  void updateMode()
  {
    if (lightMode)
    {
      Serial.println("update white");
      fillLeds(CRGB::White);
    }
    else
    {
      Serial.printf("updateCounter all %d enable %d disable %d alarm %d maintenance %d \n", _counter.all, _counter.enabled, _counter.disabled, _counter.alarm, _counter.maintenance);
      CRGB color = getStatusColor(_counter);

      fillLeds(color);
    }
  }

  void setCounter(Counter counter)
  {
    Serial.printf("setCounter all %d enable %d disable %d alarm %d maintenance %d \n", counter.all, counter.enabled, counter.disabled, counter.alarm, counter.maintenance);
    _counter = counter;
  }

  CRGB getStatusColor(Counter counter)
  {
    uint8_t hue;
    uint16_t all;
    if (counter.all > 0)
    {
      all = counter.all;
    }
    else
    {
      all = 1;
    }

    if (((100 * counter.alarm) / all) > 10)
    {
      hue = HSVHue::HUE_BLUE;
    }
    else
    {
      hue = (HSVHue::HUE_GREEN * counter.enabled) / all;
    }

    return CHSV(hue, 255, 255);
  }

  void changeLightMode()
  {
    lightMode = !lightMode;
    Serial.printf("LightMode has been changed %d\n", lightMode);
  }
};

#endif