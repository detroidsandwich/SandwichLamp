#include <Esp.h>
#include "Led.h"
#include "Web.h"

Led led;
Web web;

void setup()
{
  LedData ledData;
  Serial.begin(115200);
  led.setup(&ledData);

  web.setup(ledData, [](LedData data)
            {
              led.updateData(&data);
  String request = "CALLBACK " + String(data.currentEffect) + " " + String(data.brightness);
        Serial.println(request); 
        
          Serial.println(request);
  for (byte i = 0; i < data.COUNT_MODE; i++)
  {
    EffectData effect = data.effectData[i];
    Serial.print(effect.id);
    Serial.print(effect.name);
    Serial.print(effect.type);
    Serial.print(effect.speed);
    Serial.println(effect.scale);
  }
        
        });

  String request = "INIT " + String(ledData.currentEffect) + " " + String(ledData.brightness);
  Serial.println(request);
  for (byte i = 0; i < ledData.COUNT_MODE; i++)
  {
    EffectData effect = ledData.effectData[i];
    Serial.print(effect.id);
    Serial.print(effect.name);
    Serial.print(effect.type);
    Serial.print(effect.speed);
    Serial.println(effect.scale);
  }
}

void loop()
{
  uint32_t ms = millis();
  led.update(ms);
  web.update();
}