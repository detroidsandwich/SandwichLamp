#include <Esp.h>
#include "Led.h"
#include "Web.h"

Led led;
Web web;

LedData ledData;

void setup()
{
  Serial.begin(115200);
  led.setup();
  ledData = led.getData();
  web.setup(&ledData, [](LedData *data)
            {
              led.updateData(*data);
String request = "CALLBACK " + String(data->numberEffect) + " " + String(data->brightness) + " " + String(data->speed) + " " + String(data->scale) + " ";
        Serial.println(request); });

        
String request = "INIT " + String(ledData.numberEffect) + " " + String(ledData.brightness) + " " + String(ledData.speed) + " " + String(ledData.scale) + " ";
        Serial.println(request); 

  // WebServer &webServer = web.currentWebServer();
  // webServer.setLedData(led.getData());
  // webServer.setLedDataCallback([&](LedData data)
  //                              {
  //                               led.updateData(data);
  //                                webServer.setLedData(led.getData()); });
}

void loop()
{
  uint32_t ms = millis();
  led.update(ms);
  web.update();
}