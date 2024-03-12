#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>
#include "Automat.h"

#define DHTPIN D1     // указываем пин, к которому подключен датчик температуры
#define DHTTYPE DHT11 // указываем тип датчика (в данном случае DHT11)

class DhtManager
{
    DHT dht{DHTPIN, DHTTYPE};
    Automat tickDht{UPDATE_DELAY_MS};

public:
    DhtManager() {}
    ~DhtManager() {}

    static const uint16_t UPDATE_DELAY_MS = 20000;

    void setup(void (*callback)(float temp, float hum))
    {
        dht.begin();
        updateDhtWeb(callback);
    }

    void update(uint32_t millis, void (*callback)(float temp, float hum))
    {
        if (tickDht.tick(millis))
        {
            updateDhtWeb(callback);
        }
    }

private:
    void updateDhtWeb(void (*callback)(float temp, float hum))
    {
        float temp = dht.readTemperature(); // измеряем температуру
        float hum = dht.readHumidity();     // измеряем влажность
        (*callback)(temp,hum);
    }
};

#endif