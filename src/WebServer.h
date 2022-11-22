#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <string>

const String EFFECT = "effect";
const String MODE = "mode";
const String SPEED = "speed";
const String SCALE = "scale";

class WebServer
{
  static const byte ARG_FACTOR = 32;
  ESP8266WebServer server{80};
  u_int8_t effectNumber;
  LedEffect *currentEffect;
  std::function<void(const u_int8_t effectDiv)> callBackEffectNumber = nullptr;

public:
  WebServer()
  {
    server.on("/", [&]()
              { refresh(); });
    server.onNotFound([&]()
                      { handleNotFound(); });

    server.on("/" + EFFECT, [&]()
              { handleRequest(); });
  };

  void begin()
  {
    server.begin();
    Serial.println("server is begin");
  };

  void refresh()
  {
    server.send(200, "text/html", SendHTML(currentEffect, effectNumber));
  }

  void handleClient() { server.handleClient(); }

  void setEffectCallback(std::function<void(const u_int8_t effectDiv)> fn) { callBackEffectNumber = fn; }
  void setEffectNumber(u_int8_t effectNumber) { this->effectNumber = effectNumber; }
  void setEffectLink(LedEffect *ledEffect) { currentEffect = ledEffect; }

  void handleRequest()
  {
    // for (int i = 0; i < server.args(); i++)
    // {
    //   String message = "Arg nº" + (String)i + " –> "; // добавить текущее значение счетчика
    //   message += server.argName(i) + ": ";            // получить имя параметра
    //   message += server.arg(i);                       // получить значение параметра
    //                                                   // message += server.arg(i).substring(1, 3) + " ";
    //                                                   // message += server.arg(i).c_str();
    //   // message += server.arg(i).toInt();
    //   Serial.println(message);
    // }

    uint8_t mode = server.arg(MODE).toInt();
    uint8_t speed = server.arg(SPEED).toInt();
    uint8_t scale = server.arg(SCALE).toInt();

    if (mode != effectNumber)
    {
      callBackEffectNumber(mode);
    }
    currentEffect->setSpeed(speed);
    currentEffect->setScale(scale);
    refresh();
  }

private:
  void handleNotFound()
  {
    server.send(404, "text/plain", "Not found");
  }

  static String SendHTML(LedEffect *ledEffect, u_int8_t effectNumber)
  {
    String name = ledEffect->getName();
    u_int8_t speed = ledEffect->getSpeed();
    u_int8_t scale = ledEffect->getScale();

    String ptr = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"><title>SandwichLamp</title><style>html {font-family: Helvetica;display: inline-block;margin: 0px auto;text-align: center;}body {margin-top: 50px;}h1 {color: #444444;margin: 50px auto 30px;}h3 {color: #444444;margin-bottom: 50px;}.button {display: block;width: 80px;background-color: #1a99bc;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}.button-on {background-color: #1a99bc;}.button-on:active {background-color: #1989a8;}.button-off {background-color: #34495e;}.button-off:active {background-color: #2c3e50;}.center-item {display: block;text-align: center;}p {font-size: 14px;color: #888;margin-bottom: 10px;}</style></head><body><h1>ESP8266 Web Server</h1><h3>Using Access Point(AP) Mode</h3><h3>MODE</h3><table style=\"display: inline\"><tr><td><a class=\"button button-on\" href=\"\n";
    ptr += createLink(effectNumber - 1, speed, scale);
    ptr += "\">-</a></td><td style=\"padding: 0 20px\"><h3>\n";
    ptr += name;
    ptr += "</h3></td><td><a class=\"button button-on\" href=\"\n";
    ptr += createLink(effectNumber + 1, speed, scale);
    ptr += "\">+</a></td></tr></table><h3>SPEED</h3><table style=\"display: inline\"><tr><td><a class=\"button button-on\" href=\"\n";
    ptr += createLink(effectNumber, speed - ARG_FACTOR, scale);
    ptr += "\">-</a></td><td style=\"padding: 0 20px\"><h3>\n";
    ptr += speed / ARG_FACTOR;
    ptr += "</h3></td><td><a class=\"button button-on\" href=\"\n";
    ptr += createLink(effectNumber, speed + ARG_FACTOR, scale);
    ptr += "\">+</a></td></tr></table><h3>SCALE</h3><table style=\"display: inline\"><tr><td><a class=\"button button-on\" href=\"\n";
    ptr += createLink(effectNumber, speed, scale - ARG_FACTOR);
    ptr += "\">-</a></td><td style=\"padding: 0 20px\"><h3>\n";
    ptr += scale / ARG_FACTOR;
    ptr += "</h3></td><td><a class=\"button button-on\" href=\"\n";
    ptr += createLink(effectNumber, speed, scale + ARG_FACTOR);
    ptr += "\">+</a></td></tr></table></body></html>\n";
    // Serial.println("sendhtml is finish");
    return ptr;
  }

  static String createLink(u_int8_t mode, u_int8_t speed, u_int8_t scale)
  {
    return EFFECT + "?" + MODE + "=" + mode + "&" + SPEED + "=" + speed + "&" + SCALE + "=" + scale;
  }
};

#endif