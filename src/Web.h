#ifndef WEB_H
#define WEB_H

#include <ESP8266WiFi.h>
#include "WebServer.h"
#include <ESP8266mDNS.h>
#include <WebPage.h>

// REMOVE IT
const char *ssid = "ssid";
const char *password = "password";

/* Настройки IP адреса */
// IPAddress apIP(172, 217, 28, 1);
// IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server{80};
String htmlPage = "";

class Web
{
  const String DOMAIN_NAME = "sandwichlamp"; // sandwichlamp.local
  // WebServer webServer;

  // ESP8266WebServer server{80};

  // String htmlPage = "";

public:
  Web() {}
  ~Web() {}

  struct LedData ledData;

  void setup(LedData *initLedData, std::function<void(LedData *)> callback)
  {
    // WiFi.mode(WIFI_AP);
    // WiFi.softAP("SandwichLamp");
    // WiFi.softAPConfig(apIP, apIP, subnet);

    ledData = *initLedData;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(DOMAIN_NAME))
    {
      Serial.println("MDNS responder started");
    }
    else
    {
      Serial.println("Error setting up MDNS responder!");
    }

    htmlPage = createWebPage(MODE_NAMES, COUNT_MODE, &ledData);

    server.on("/", []()
              { server.send(200, "text/html", htmlPage); });
    server.on("/mode", HTTP_GET, [callback, this]()
              {
                byte value = server.arg("value").toInt();
                this->ledData.numberEffect = value;
                callback(&this->ledData);
                server.send(200, "text/plain", "Mode set to " + String(value)); });

    server.on("/brightness", HTTP_GET, [callback, this]()
              {
                byte value = converWebToLedValue(server.arg("value"));
                this->ledData.brightness = value;
                callback(&this->ledData);
                server.send(200, "text/plain", "Brightness set to " + String(value)); });

    server.on("/speed", HTTP_GET, [callback, this]()
              {
                byte value = converWebToLedValue(server.arg("value"));
                this->ledData.speed = value;
                callback(&this->ledData);
                server.send(200, "text/plain", "Speed set to " + String(value)); });

    server.on("/scale", HTTP_GET, [callback, this]()
              {
                byte value = converWebToLedValue(server.arg("value"));
                this->ledData.scale = value;
                callback(&this->ledData);
                server.send(200, "text/plain", "Scale set to " + String(value)); });

    // Запуск сервера
    server.begin();
    Serial.println("Server started");

    // webServer.begin();

    MDNS.addService("http", "tcp", 80);
  }

  void update()
  {
    // MDNS.update();
    // webServer.handleClient();
    server.handleClient();
  }

  void updateCallbackData(LedData ledData, std::function<void(LedData *)> callback)
  {
  }

  byte converWebToLedValue(String webValue)
  {
    int value = webValue.toInt() * FACTOR;
    if (value >= MAX_SLIDER_VALUE)
    {
      return MAX_SLIDER_VALUE;
    }

    return value;
  }

  // WebServer &currentWebServer()
  // {
  //   return webServer;
  // }
};

#endif