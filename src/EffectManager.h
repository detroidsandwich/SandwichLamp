#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "ledeffect/LedEffect.h"
#include "ledeffect/FireEffect.h"

class EffectManager
{
public:
    static const int COUNT_MODE = 4; // don't foget update this

    void setEffectNumber(uint8_t effectNumber) { updateEffect(safeNumberMode(effectNumber)); }

    u_int8_t getCurrentEffectNumber() { return m_currentMode; }
    LedEffect *getCurrentEffect() { return currentEffect; }

    void update(uint32_t tick) { currentEffect->update(tick); }

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
        case 3:
            currentEffect = new Fire();
            break;
        }
        // don't foget update COUNT_MODE
        // Serial.print("updateEffect = ");
        // Serial.println(effectNumber);
    }
};

#endif