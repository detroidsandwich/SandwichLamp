#ifndef WEB_H
#define WEB_H

// #pragma once

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "WebPage.h"

// class DhtManager;

// REMOVE IT
const char *ssid = "ssid";
const char *password = "password";

/* Настройки IP адреса */
// IPAddress apIP(172, 217, 28, 1);
// IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server{80};

class Web
{
  const String DOMAIN_NAME = "sandwichlamp"; // sandwichlamp.local

public:
  Web() {}
  ~Web() {}

  LedData data;
  float temp;
  float humidity;

  void setup(LedData initLedData, std::function<void(LedData)> callback)
  {
    // WiFi.mode(WIFI_AP);
    // WiFi.softAP("SandwichLamp");
    // WiFi.softAPConfig(apIP, apIP, subnet);

    data = initLedData;

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

    server.on("/", [this]()
              { server.send(200, "text/html", createWebPage(data)); });

    server.on("/mode", HTTP_GET, [callback, this]()
              {
                byte value = server.arg("value").toInt();
                data.currentEffect = value;
                callback(data);
                server.send(200, "text/plain", "Mode set to " + String(value)); });

    server.on("/brightness", HTTP_GET, [callback, this]()
              {
                byte value = server.arg("value").toInt();
                data.brightness = value;
                callback(data);
                server.send(200, "text/plain", "Brightness set to " + String(value)); });

    server.on("/speed", HTTP_GET, [callback, this]()
              {
                byte value = server.arg("value").toInt();
                data.effectData[data.currentEffect].speed = value;
                callback(data);
                server.send(200, "text/plain", "Speed set to " + String(value)); });

    server.on("/scale", HTTP_GET, [callback, this]()
              {
                byte value = server.arg("value").toInt();
                data.effectData[data.currentEffect].scale = value;
                callback(data);
                server.send(200, "text/plain", "Scale set to " + String(value)); });

    server.on("/dht", HTTP_GET, [this]()
              {
                StaticJsonDocument<200> doc;
                doc["temperature"] = String(temp);
                doc["humidity"] = humidity;
                String output;
                serializeJson(doc, output);
                server.send(200, "application/json", output); });

    // Запуск сервера
    server.begin();
    Serial.println("Server started");

    MDNS.addService("http", "tcp", 80);
  }

  void update()
  {
    // MDNS.update();
    server.handleClient();
  }

  void updateDht(float temperature, float humidity)
  {
    this->temp = temperature;
    this->humidity = humidity;
  }
};

#endif