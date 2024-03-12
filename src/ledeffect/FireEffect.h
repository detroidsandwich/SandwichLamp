#ifndef FIRE_EFFECT_H
#define FIRE_EFFECT_H

#include "LedSettings.h"
#include "LedEffect.h"

class Fire : public LedEffect
{
	const bool SPARKLES = true;
	unsigned char line[matrixWidth];
	int pcnt = 0;
	bool loadingFlag = true;
	unsigned char matrixValue[8][16];

	// these values are substracetd from the generated values to give a shape to the animation
	const unsigned char valueMask[8][16] = {
		{32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32},
		{64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64},
		{96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96},
		{128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128},
		{160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160},
		{192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192},
		{255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255},
		{255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}};

	// these are the hues for the fire,
	// should be between 0 (red) to about 25 (yellow)
	const unsigned char hueMask[8][16] = {
		{1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1},
		{1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1},
		{1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1},
		{1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1},
		{1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1},
		{0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0},
		{0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0},
		{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};

public:
	Fire() : LedEffect("Fire") {}

	void update(uint32_t tick) override
	{
		if (loadingFlag)
		{
			loadingFlag = false;
			// FastLED.clear();
			generateLine();
		}
		if (pcnt >= 100)
		{
			shiftUp();
			generateLine();
			pcnt = 0;
		}
		drawFrame(pcnt);
		pcnt += (speed);
	}

private:
	void generateLine()
	{
		for (uint8_t x = 0; x < matrixWidth; x++)
		{
			line[x] = random(64, 255);
		}
	}

	void shiftUp()
	{
		for (uint8_t y = matrixHeight - 1; y > 0; y--)
		{
			for (uint8_t x = 0; x < matrixWidth; x++)
			{
				uint8_t newX = x;
				if (x > 15)
					newX = x - 15;
				if (y > 7)
					continue;
				matrixValue[y][newX] = matrixValue[y - 1][newX];
			}
		}

		for (uint8_t x = 0; x < matrixWidth; x++)
		{
			uint8_t newX = x;
			if (x > 15)
				newX = x - 15;
			matrixValue[0][newX] = line[newX];
		}
	}

	void drawFrame(int pcnt)
	{
		int nextv;

		// each row interpolates with the one before it
		for (unsigned char y = matrixHeight - 1; y > 0; y--)
		{
			for (unsigned char x = 0; x < matrixWidth; x++)
			{
				uint8_t newX = x;
				if (x > 15)
					newX = x - 15;
				if (y < 8)
				{
					nextv =
						(((100.0 - pcnt) * matrixValue[y][newX] + pcnt * matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

					CRGB color = CHSV(
						scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
						255,												// S
						(uint8_t)max(0, nextv)								// V
					);

					leds[XYsafe(x, y)] = color;
				}
				else if (y == 8 && SPARKLES)
				{
					if (random(0, 20) == 0 && XYsafe(x, y - 1) != 0)
						leds[XYsafe(x, y)] = colorXY(x, y - 1);
					else
						leds[XYsafe(x, y)] = 0;
				}
				else if (SPARKLES)
				{

					// старая версия для яркости
					if (colorXY(x, y - 1) > 0)
						leds[XYsafe(x, y)] = colorXY(x, y - 1);
					else
						leds[XYsafe(x, y)] = 0;
				}
			}
		}
		// first row interpolates with the "next" line
		for (unsigned char x = 0; x < matrixWidth; x++)
		{
			uint8_t newX = x;
			if (x > 15)
				newX = x - 15;
			CRGB color = CHSV(
				scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])),							   // H
				255,																		   // S
				(uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
			);
			leds[XYsafe(newX, 0)] = color;
		}
	}
};

#endif