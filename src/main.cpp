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

  WebServer &webServer = web.currentWebServer();
  webServer.setEffect(led.currentEffect());

  webServer.setEffectCallback([&](int8_t effectDiv)
                              { led.shiftEffectNumber(effectDiv);
                                webServer.setEffect(led.currentEffect());
                                webServer.refresh(); });
}

void loop()
{
  led.update();
  web.update();
}