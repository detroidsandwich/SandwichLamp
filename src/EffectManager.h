#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "ledeffect/LedEffect.h"
#include "ledeffect/FireEffect.h"
#include "ledeffect/LedNoiseEffect.h"
#include "LedData.h"

class EffectManager
{
public:
    void setup(LedData *data)
    {
        updateEffect(data->currentEffect);
        currentMode = data->currentEffect;

        // EffectData effectData = data->effectData[data->currentEffect];
        currentEffect->setEffectData(data->effectData[data->currentEffect]);
    }

    void update(uint32_t tick) { currentEffect->update(tick); }

    void updateData(LedData *data)
    {
        if (currentMode != data->currentEffect)
        {
            updateEffect(data->currentEffect);
            currentMode = data->currentEffect;
        };

        // EffectData effectData = data->effectData[data->currentEffect];
        currentEffect->setEffectData(data->effectData[data->currentEffect]);
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
        case 6:
            currentEffect = new RainbowNoise();
            break;
        case 7:
            currentEffect = new RainbowStripeNoise();
            break;
        case 8:
            currentEffect = new ZebraNoise();
            break;
        case 9:
            currentEffect = new PlasmaNoise();
            break;
        case 10:
            currentEffect = new CloudNoise();
            break;
        case 11:
            currentEffect = new LavaNoise();
            break;
        }
        // don't foget update COUNT_MODE
    }
};

#endif