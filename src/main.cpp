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
}

void loop()
{
  led.update();
  web.loop();
}