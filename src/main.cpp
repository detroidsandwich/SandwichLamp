#include <Esp.h>
#include "Led.h"
#include "Web.h"

Led led;
Web web;

void setup()
{
  Serial.begin(115200);
  led.setup();
  web.setup();
  EffectManager &effectManager = led.getEffectManager();
  WebServer &webServer = web.currentWebServer();

  webServer.setEffectLink(effectManager.getCurrentEffect());
  webServer.setEffectNumber(effectManager.getCurrentEffectNumber());

  webServer.setEffectCallback([&](u_int8_t effectNumber)
                              { effectManager.setEffectNumber(effectNumber);
                                webServer.setEffectLink(effectManager.getCurrentEffect());
                                webServer.setEffectNumber(effectManager.getCurrentEffectNumber()); });
}

void loop()
{
  uint32_t ms = millis();
  led.update(ms);
  web.update();
}