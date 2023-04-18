#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "ledeffect/LedEffect.h"
#include "ledeffect/FireEffect.h"
#include "LedData.h"


const int COUNT_MODE = 5; // don't foget update this
String MODE_NAMES[COUNT_MODE] = {"RotateRainbow", "SparklesRoutine", "RainbowVertical", "Fire", "Matrix"};

class EffectManager
{
public:

    u_int8_t getCurrentEffectNumber() { return m_currentMode; }
    LedEffect *getCurrentEffect() { return currentEffect; }

    void update(uint32_t tick) { currentEffect->update(tick); }

    void updateData(LedData &data)
    {
        if (m_currentMode != data.numberEffect)
        {
            updateEffect(safeNumberMode(data.numberEffect));
        };
        currentEffect->setSpeed(data.speed);
        currentEffect->setScale(data.scale);
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
    LedEffect *currentEffect = new WhiteEffect();

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
        case 4:
            currentEffect = new Matrix();
            break;
        }
        // don't foget update COUNT_MODE
        // Serial.print("updateEffect = ");
        // Serial.println(effectNumber);
    }
};

#endif