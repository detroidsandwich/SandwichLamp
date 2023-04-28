#ifndef LED_NOISE_EFFECT_H
#define LED_NOISE_EFFECT_H

#include "LedSettings.h"
#include "LedEffect.h"

class LedNoiseEffect : public LedEffect
{
protected:
    boolean loadingFlag = true;
    uint8_t noise[MATRIX_WIDTH][MATRIX_HEIGHT];

    uint16_t x;
    uint16_t y;
    uint16_t z;

    uint8_t MAX_DIMENSION = MATRIX_WIDTH;

    CRGBPalette16 currentPalette{PartyColors_p}; // PartyColors_p
    uint8_t colorLoop = 1;
    uint8_t ihue = 0;

    uint8_t FACTOR_SPEED = 10;
    uint8_t FACTOR_SCALE = 5;

    uint8_t MIN_SPEED = 2;
    uint8_t MIN_SCALE = 5;

public:
    LedNoiseEffect(String name) : LedEffect(name) {}

    virtual ~LedNoiseEffect() {}

    void setSpeed(uint8_t speed) override { m_speed = speed / FACTOR_SPEED + MIN_SPEED; }

    void setScale(uint8_t scale) override { m_scale = scale / FACTOR_SCALE + MIN_SCALE; }

protected:
    void fillNoiseLED()
    {
        uint8_t dataSmoothing = 0;
        if (m_speed < 50)
        {
            dataSmoothing = 200 - (m_speed * 4);
        }
        for (int i = 0; i < MAX_DIMENSION; i++)
        {
            int ioffset = m_scale * i;
            for (int j = 0; j < MAX_DIMENSION; j++)
            {
                int joffset = m_scale * j;

                uint8_t data = inoise8(x + ioffset, y + joffset, z);

                data = qsub8(data, 16);
                data = qadd8(data, scale8(data, 39));

                if (dataSmoothing)
                {
                    uint8_t olddata = noise[i][j];
                    uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                    data = newdata;
                }

                noise[i][j] = data;
            }
        }
        z += m_speed;

        // apply slow drift to X and Y, just for visual variation.
        x += m_speed / 8;
        y -= m_speed / 16;

        for (int i = 0; i < MATRIX_WIDTH; i++)
        {
            for (int j = 0; j < MATRIX_HEIGHT; j++)
            {
                uint8_t index = noise[j][i];
                uint8_t bri = noise[i][j];
                // if this palette is a 'loop', add a slowly-changing base value
                if (colorLoop)
                {
                    index += ihue;
                }
                // brighten up, as the color palette itself often contains the
                // light/dark dynamic range desired
                if (bri > 127)
                {
                    bri = 255;
                }
                else
                {
                    bri = dim8_raw(bri * 2);
                };
                // CRGB ColorFromPalette( const CRGBPalette16& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
                CRGB color = ColorFromPalette(currentPalette, index, bri, NOBLEND);
                // drawPixelXY(i, j, color); // leds[getPixelNumber(i, j)] = color;
                leds[XYsafe(i, j)] = color;
            }
        }
        ihue += 1;
    }

    void fillnoise8()
    {
        for (int i = 0; i < MAX_DIMENSION; i++)
        {
            int ioffset = m_scale * i;
            for (int j = 0; j < MAX_DIMENSION; j++)
            {
                int joffset = m_scale * j;
                noise[i][j] = inoise8(x + ioffset, y + joffset, z);
            }
        }
        z += m_speed;
    }
};
//
//  MadnessNoise (NOT USED)
//
class MadnessNoise : public LedNoiseEffect
{

public:
    MadnessNoise() : LedNoiseEffect("MadnessNoise") {}

    void update(uint32_t tick) override
    {
        fillnoise8();
        for (int i = 0; i < MATRIX_WIDTH; i++)
        {
            for (int j = 0; j < MATRIX_HEIGHT; j++)
            {
                CRGB thisColor = CHSV(noise[j][i], 255, noise[i][j]);
                leds[XYsafe(i, j)] = thisColor;
            }
        }
        // ihue += 1;
    }
};
//
//  RainbowNoise
//
class RainbowNoise : public LedNoiseEffect
{

public:
    RainbowNoise() : LedNoiseEffect("RainbowNoise") {}

    void update(uint32_t tick) override
    {
        Serial.println(name +" "+ String(m_speed) +" "+ String(m_scale));
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = RainbowColors_p;
            colorLoop = 1;
        }
        fillNoiseLED();
    }
};
//
//  RainbowStripeNoise
//
class RainbowStripeNoise : public LedNoiseEffect
{
    uint8_t FACTOR_SPEED = 22;
    uint8_t FACTOR_SCALE = 16;

    uint8_t MIN_SPEED = 2;
    uint8_t MIN_SCALE = 4;
public:
    RainbowStripeNoise() : LedNoiseEffect("RainbowStripeNoise") {}

    void setSpeed(uint8_t speed) override { m_speed = speed / FACTOR_SPEED + MIN_SPEED; }

    void setScale(uint8_t scale) override { m_scale = scale / FACTOR_SCALE + MIN_SCALE; }

    void update(uint32_t tick) override
    {
        Serial.println(name +" "+ String(m_speed) +" "+ String(m_scale));
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = RainbowStripeColors_p;
            colorLoop = 1;
        }
        fillNoiseLED();
    }
};
//
//  ZebraNoise
//
class ZebraNoise : public LedNoiseEffect
{
        uint8_t FACTOR_SPEED = 22;
    uint8_t FACTOR_SCALE = 16;

    uint8_t MIN_SPEED = 2;
    uint8_t MIN_SCALE = 4;
public:
    ZebraNoise() : LedNoiseEffect("ZebraNoise") {}

    void setSpeed(uint8_t speed) override { m_speed = speed / FACTOR_SPEED + MIN_SPEED; }

    void setScale(uint8_t scale) override { m_scale = scale / FACTOR_SCALE + MIN_SCALE; }

    void update(uint32_t tick) override
    {
        if (loadingFlag)
        {
            loadingFlag = false;
            // 'black out' all 16 palette entries...
            fill_solid(currentPalette, 16, CRGB::Black);
            // and set every fourth one to white.
            currentPalette[0] = CRGB::White;
            currentPalette[4] = CRGB::White;
            currentPalette[8] = CRGB::White;
            currentPalette[12] = CRGB::White;
            colorLoop = 1;
        }
        fillNoiseLED();
    }
};
//
//  ForestNoise (NOT USED)
//
class ForestNoise : public LedNoiseEffect
{

public:
    ForestNoise() : LedNoiseEffect("ForestNoise") {}

    void update(uint32_t tick) override
    {
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = ForestColors_p;
            colorLoop = 0;
        }
        fillNoiseLED();
    }
};
//
//  OceanNoise (NOT USED)
//
class OceanNoise : public LedNoiseEffect
{

public:
    OceanNoise() : LedNoiseEffect("OceanNoise") {}

    void update(uint32_t tick) override
    {
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = OceanColors_p;
            colorLoop = 0;
        }

        fillNoiseLED();
    }
};
//
//  PlasmaNoise
//
class PlasmaNoise : public LedNoiseEffect
{

public:
    PlasmaNoise() : LedNoiseEffect("PlasmaNoise") {}

    void update(uint32_t tick) override
    {
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = PartyColors_p;
            colorLoop = 1;
        }
        fillNoiseLED();
    }
};
//
//  CloudNoise
//
class CloudNoise : public LedNoiseEffect
{

public:
    CloudNoise() : LedNoiseEffect("CloudNoise") {}

    void update(uint32_t tick) override
    {
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = CloudColors_p;
            colorLoop = 0;
        }
        fillNoiseLED();
    }
};
//
//  LavaNoise
//
class LavaNoise : public LedNoiseEffect
{

public:
    LavaNoise() : LedNoiseEffect("LavaNoise") {}

    void update(uint32_t tick) override
    {
        if (loadingFlag)
        {
            loadingFlag = false;
            currentPalette = LavaColors_p;
            colorLoop = 0;
        }
        fillNoiseLED();
    }
};

#endif