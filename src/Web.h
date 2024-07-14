#ifndef WEB_H
#define WEB_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "Config.h"
#include "Counter.h"

class Web
{
  WiFiClient client;
  Automat updateCounterTick{60000};
  std::function<void(Counter)> _callback;

public:
  Web() {}
  ~Web() {}

  // Counter counter;

  void setup(std::function<void(Counter)> callback)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int8_t retryCount = 30;
    while (WiFi.status() != WL_CONNECTED && retryCount > 0)
    {
      delay(250);
      retryCount--;
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    _callback = callback;

    update();
  }

  void update(uint32_t ms)
  {
    if (updateCounterTick.tick(ms))
    {
      update();
    }
  }

  void update()
  {
    Counter counter = requestUrl();
    Serial.printf("requestUrl all %d enable %d disable %d alarm %d maintenance %d \n", counter.all, counter.enabled, counter.disabled, counter.alarm, counter.maintenance);
    _callback(counter);
  }

  Counter requestUrl()
  {
    Counter counter;
    HTTPClient http;

    http.begin(client, serverUrl);

    Serial.println(serverUrl);
    int httpCode = http.GET();

    if (httpCode == 200)
    {
      Serial.println("success");

      WiFiClient *stream = http.getStreamPtr();
      do
      {
        String line = stream->readStringUntil('\n');
        // Serial.println(line);
        delay(1);
        if (line.indexOf("Група 6") >= 0)
        {
          String sub = line.substring(0, 60);
          if (sub.indexOf("Київ") >= 0)
          {
            counter.all++;
            char status = sub.charAt(7);
            switch (status)
            {
            case '1':
              counter.enabled++;
              break;

            case '2':
              counter.disabled++;
              break;

            case '3':
              counter.maintenance++;
              break;

            case '4':
              counter.maintenance++;
              break;

            case '-':
              counter.alarm++;
              break;
            }
          }
        }
      } while (stream->connected() && stream->available());
    }
    else
    {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
    }

    // Serial.printf("all %d enable %d disable %d alarm %d maintenance %d \n", counter.all, counter.enabled, counter.disabled, counter.alarm, counter.maintenance);

    http.end(); // Free the resources
    return counter;
  }
};

#endif