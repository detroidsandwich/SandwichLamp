#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "LedEffect.h"

class EffectManager
{
public:
    static const int COUNT_MODE = 3; // don't foget update this

    void shiftEffectNumber(uint8_t effectNumber)
    {
        updateEffect(safeNumberMode(m_currentMode + effectNumber));
    }

    LedEffect* getCurrentEffect(){
        return currentEffect;
    }

    void update(uint32_t tick)
    {
        currentEffect->update(tick);
    }

    void clear()
    {
        for (size_t i = 0; i < sizeof(leds); i++)
        {
            leds[i] = CRGB::Black;
        }
    }

    static int8_t safeNumberMode(int8_t i)
    {
        if (i >= COUNT_MODE)
            return 0;
        if (i < 0)
            return COUNT_MODE - 1;
        return i;
    }

private:
    int m_currentMode = 0;
    LedEffect *currentEffect = new RotateRainbow();

    void updateEffect(uint8_t effectNumber)
    {
        m_currentMode = effectNumber;
        delete currentEffect;
        switch (effectNumber)
        {
        case 0:
            currentEffect = new RotateRainbow();
            break;
        case 1:
            currentEffect = new SparklesRoutine();
            break;
        case 2:
            currentEffect = new RainbowVertical();
            break;
        }
        Serial.print("updateEffect = ");
        Serial.println(effectNumber);
    }
};

#endif