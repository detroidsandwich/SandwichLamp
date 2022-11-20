#ifndef WEB_H
#define WEB_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WebServer.h"
#include "Automat.h"
#include <functional>
#include <ESP8266mDNS.h>

// REMOVE IT
const char *ssid = "ssid";
const char *password = "password";

/* Настройки IP адреса */
// IPAddress apIP(172, 217, 28, 1);
// IPAddress subnet(255, 255, 255, 0);

// Automat updateServer(200);

ESP8266WebServer server(80);

class Web
{
public:
  Web() {}
  ~Web() {}

  void setup()
  {
    // WiFi.mode(WIFI_AP);
    // WiFi.softAP("SandwichLamp");
    // WiFi.softAPConfig(apIP, apIP, subnet);

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

    if (MDNS.begin("sandwichlamp"))
    {
      Serial.println("MDNS responder started");
    }
    else
    {
      Serial.println("Error setting up MDNS responder!");
    }

    WebServer webServer{server};

    MDNS.addService("http", "tcp", 80);
  }

  void loop()
  {
    MDNS.update();
    server.handleClient();
  }
};

#endif