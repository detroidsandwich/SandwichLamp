# SandwichLamp
## _In the process of development._

This project is a fork of Alex Gyver's project.
See original here : https://github.com/AlexGyver/GyverLamp

_Transfer and addition of logic from Arduino IDE to PlatformIO._
https://platformio.org/platformio-ide

To start the project, you need to create a file with the login and password for your Wi-Fi:
In root, create a file named `secrets.h`

```c++
#ifndef SECRETS_H
#define SECRETS_H

// Wi-Fi credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";     
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD"; 

#endif
```