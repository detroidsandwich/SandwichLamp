#include <Esp.h>
#include "Led.h"
#include "Web.h"
#include "button/Button.h"

Led led;
Web web;
Button flashButton{D3};

void setup()
{
  Serial.begin(115200);
  flashButton.begin();
  led.setup();

  web.setup([](Counter counter)
            {  
            Serial.printf("callback %d", counter.all);
            led.setCounter(counter); 
            led.updateMode(); });
};

void loop()
{
  uint32_t ms = millis();
  if (flashButton.updateAndCheckPeleased())
  {
    Serial.println("!!!!!Button released!!!!!");
    led.changeLightMode();
    led.updateMode();
  }
  led.update();
  web.update(ms);
};