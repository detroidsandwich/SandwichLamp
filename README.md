# SandwichLamp
## _In the process of development._

This project is a fork of Alex Gyver's project.
See original here : https://github.com/AlexGyver/GyverLamp

_Transfer and addition of logic from Arduino IDE to PlatformIO._
https://platformio.org/platformio-ide

## Before running!
_Add `Config.h` file with the following code_
```
#ifndef CONFIG_H
#define CONFIG_H

static const char *ssid = "Name_your_wifi";
static const char *password = "password";
static const char *serverUrl = "url_svitlobot";

#endif
```