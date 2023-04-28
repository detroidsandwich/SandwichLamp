#include <Esp.h>
#include "Led.h"
#include "Web.h"
#include "DhtManager.h"

Led led;
Web web;
DhtManager dht;

void setup()
{
  LedData ledData;
  Serial.begin(115200);
  led.setup(&ledData);
  web.setup(ledData, [](LedData data)
            { led.updateData(&data); });
  dht.setup([](float temp, float hum)
            { web.updateDht(temp, hum); });
};

void loop()
{
  uint32_t ms = millis();
  led.update(ms);
  web.update();
  dht.update(ms, [](float temp, float hum)
             { web.updateDht(temp, hum); });
};