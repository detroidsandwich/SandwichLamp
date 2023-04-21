#ifndef LED_DATA_H
#define LED_DATA_H

const byte MAX_SLIDER_VALUE = 255;
const byte FACTOR_VALUE = 16;

String EFFECT_TYPE_NORMAL = "normal";

struct EffectData
{
    byte id;
    String name;
    String type;
    byte speed;
    byte scale;
};

struct LedData
{
    static const byte COUNT_MODE = 5; // Размер массива
    EffectData effectData[COUNT_MODE] = {
        {0, "RotateRainbow", EFFECT_TYPE_NORMAL, 5 * FACTOR_VALUE, 1 * FACTOR_VALUE},
        {1, "SparklesRoutine", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 6 * FACTOR_VALUE},
        {2, "RainbowVertical", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 4 * FACTOR_VALUE},
        {3, "Fire", EFFECT_TYPE_NORMAL, 1 * FACTOR_VALUE, 7 * FACTOR_VALUE},
        {4, "Matrix", EFFECT_TYPE_NORMAL, 10 * FACTOR_VALUE, 12 * FACTOR_VALUE},
    };

    byte currentEffect = 0;
    byte brightness = 3 * FACTOR_VALUE;
};

#endif