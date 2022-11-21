#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <string>

const String EFFECT_NEXT = "effectNext";
const String EFFECT_PREV = "effectPrev";

class WebServer
{
  ESP8266WebServer server{80};
  LedEffect *currentEffect;
  std::function<void(const int8_t effectDiv)> callBackAddEffect = nullptr;

public:
  WebServer()
  {
    server.on("/", [&]()
              { refresh(); });
    server.onNotFound([&]()
                      { handleNotFound(); });

    server.on("/" + EFFECT_NEXT, [&]()
              { callBackAddEffect(1); });
    server.on("/" + EFFECT_PREV, [&]()
              { callBackAddEffect(-1); });
  };

  void begin()
  {
    server.begin();
    Serial.println("server is begin");
  };

  void refresh()
  {
    server.send(200, "text/html", SendHTML(currentEffect));
  }

  void handleClient()
  {
    server.handleClient();
  }

  void setEffectCallback(std::function<void(const int8_t effectDiv)> fn)
  {
    callBackAddEffect = fn;
  }

  void setEffect(LedEffect *ledEffect)
  {
    currentEffect = ledEffect;
  }

private:
  void handleNotFound()
  {
    server.send(404, "text/plain", "Not found");
  }

  static String SendHTML(LedEffect *ledEffect)
  {
    String ptr = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"><title>SandwichLamp</title><style>html {font-family: Helvetica;display: inline-block;margin: 0px auto;text-align: center;}body {margin-top: 50px;}h1 {color: #444444;margin: 50px auto 30px;}h3 {color: #444444;margin-bottom: 50px;}.button {display: block;width: 80px;background-color: #1a99bc;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}.button-on {background-color: #1a99bc;}.button-on:active {background-color: #1989a8;}.button-off {background-color: #34495e;}.button-off:active {background-color: #2c3e50;}.center-item {display: block;text-align: center;}p {font-size: 14px;color: #888;margin-bottom: 10px;}</style></head><body><h1>ESP8266 Web Server</h1><h3>Using Access Point(AP) Mode</h3><table style=\"display: inline\"><tr><td><a class=\"button button-on\" href=\"\n";
    ptr += EFFECT_PREV;
    ptr += "\">-</a></td><td style=\"padding: 0 20px\"><h3>\n";
    ptr += ledEffect->getName();
    ptr += "</h3></td><td><a class=\"button button-on\" href=\"\n";
    ptr += EFFECT_NEXT;
    ptr += "\">+</a></td></tr></table></body></html>\n";
    return ptr;
  }
};

#endif