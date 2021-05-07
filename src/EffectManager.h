#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "LedEffect.h"

class EffectManager
{
public:
    const int COUNT_MODE = 3; // don't foget update this
    int m_currentMode = 0;
    Effect *currentEffect = new RotateRainbow();

    void nextEffect()
    {
        updateEffect(safeNumberMode(++m_currentMode));
    }

    void previousEffect()
    {
        updateEffect(safeNumberMode(--m_currentMode));
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

private:
    void updateEffect(uint8_t effectNumber)
    {
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
        Serial.print("updateEffect");
        Serial.print(effectNumber);
        Serial.println();
    }

    uint8_t safeNumberMode(int8_t i)
    {
        if (i >= COUNT_MODE)
            m_currentMode = 0;
        if (i < 0)
            m_currentMode = COUNT_MODE - 1;
        return m_currentMode;
    }
};

#endif