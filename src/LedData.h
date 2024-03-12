#ifndef LED_DATA_H
#define LED_DATA_H

const uint8_t MAX_SLIDER_VALUE = 255;
const uint8_t FACTOR_VALUE = 16;

uint8_t EFFECT_TYPE_NORMAL = 1;
uint8_t EFFECT_TYPE_COLOR = 2;

struct EffectData
{
    uint8_t id;
    String name;
    uint8_t type;
    uint8_t speed;
    uint8_t scale;
    uint8_t hue;
};

struct LedData
{
    static const uint8_t COUNT_MODE = 12; // Размер массива
    EffectData effectData[COUNT_MODE] = {
        {0, "RotateRainbow", EFFECT_TYPE_NORMAL, 5 * FACTOR_VALUE, 1 * FACTOR_VALUE},
        {1, "SparklesRoutine", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 6 * FACTOR_VALUE},
        {2, "RainbowVertical", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 4 * FACTOR_VALUE},
        {3, "Fire", EFFECT_TYPE_NORMAL, 1 * FACTOR_VALUE, 7 * FACTOR_VALUE},
        {4, "Matrix", EFFECT_TYPE_COLOR, 10 * FACTOR_VALUE, 12 * FACTOR_VALUE, HUE_GREEN},
        {5, "ColorBlink", EFFECT_TYPE_COLOR, 10 * FACTOR_VALUE, 12 * FACTOR_VALUE, HUE_GREEN},
        {6, "RainbowNoise", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 3 * FACTOR_VALUE},
        {7, "RainbowStripeNoise", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 10 * FACTOR_VALUE},
        {8, "ZebraNoise", EFFECT_TYPE_NORMAL, 5 * FACTOR_VALUE, 12 * FACTOR_VALUE},
        {9, "PlasmaNoise", EFFECT_TYPE_NORMAL, 1 * FACTOR_VALUE, 10 * FACTOR_VALUE},
        {10, "CloudNoise", EFFECT_TYPE_NORMAL, 4 * FACTOR_VALUE, 12 * FACTOR_VALUE},
        {11, "LavaNoise", EFFECT_TYPE_NORMAL, 2 * FACTOR_VALUE, 8 * FACTOR_VALUE}
    };

    uint8_t currentEffect = 0;
    uint8_t brightness = 3 * FACTOR_VALUE;
};

#endif