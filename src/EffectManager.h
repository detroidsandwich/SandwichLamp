#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "ledeffect/LedEffect.h"
#include "ledeffect/FireEffect.h"
#include "LedData.h"

class EffectManager
{
public:

    void setup(LedData *data)
    {
        updateEffect(data->currentEffect);
        currentMode = data->currentEffect;

        EffectData effectData = data->effectData[data->currentEffect];
        currentEffect->setSpeed(effectData.speed);
        currentEffect->setScale(effectData.speed);
    }

    void update(uint32_t tick) { currentEffect->update(tick); }

    void updateData(LedData *data)
    {
        if (currentMode != data->currentEffect)
        {
            updateEffect(data->currentEffect);
            currentMode = data->currentEffect;
        };

        EffectData effectData = data->effectData[data->currentEffect];
        currentEffect->setSpeed(effectData.speed);
        currentEffect->setScale(effectData.scale);
    }

    static int8_t safeNumberMode(int8_t i)
    {
        if (i >= LedData::COUNT_MODE)
            return 0;
        if (i < 0)
            return LedData::COUNT_MODE - 1;
        return i;
    }

private:
    LedEffect *currentEffect;
    byte currentMode;

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
        case 3:
            currentEffect = new Fire();
            break;
        case 4:
            currentEffect = new Matrix();
            break;
        case 5:
            currentEffect = new ColorBlink();
            break;
        }
        // don't foget update COUNT_MODE
    }
};

#endif