#ifndef LED_EFFECT_H
#define LED_EFFECT_H

#include "LedSettings.h"

uint8_t matrixWidth = MATRIX_WIDTH; // change to led settings
uint8_t matrixHeight = MATRIX_HEIGHT;

// Abstract class for implement
class Effect
{
public:
	virtual ~Effect(){};

	inline virtual void update(uint32_t tick) { Serial.println("not implement"); };

	void setSpeed(uint8_t speed) { m_scale = speed; }
	uint8_t getSpeed() { return m_speed; }

	void setScale(uint8_t scale) { m_scale = scale; }
	uint8_t getScale() { return m_scale; }

	uint8_t m_speed = 30; // default speed 0..255
	uint8_t m_scale = 40; // default scale 0..255

protected:
	Effect() {}
};

// Effect for FastLed matrix
class RotateRainbow : public Effect
{
public:
	RotateRainbow() : Effect() {}

	void update(uint32_t tick) override
	{
		float_t iteration = tick * m_speed / 255;
		int32_t yHueDelta32 = ((int32_t)cos16(iteration * (27 / 1)) * (350 / matrixWidth));
		int32_t xHueDelta32 = ((int32_t)cos16(iteration * (39 / 1)) * (310 / matrixHeight));
		drawOneFrame(iteration / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
	}

private:
	void drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
	{
		byte lineStartHue = startHue8;
		for (byte y = 0; y < matrixHeight; y++)
		{
			lineStartHue += yHueDelta8;
			byte pixelHue = lineStartHue;
			for (byte x = 0; x < matrixWidth; x++)
			{
				pixelHue += xHueDelta8;
				leds[XY(x, y)] = CHSV(pixelHue, 255, 255);
			}
		}
	}
};

class SparklesRoutine : public Effect
{
public:
	SparklesRoutine() : Effect() {}

	void update(uint32_t tick) override
	{
		for (byte i = 0; i < m_scale; i++)
		{
			byte x = random(0, matrixWidth);
			byte y = random(0, matrixHeight);
			if (colorXY(x, y) == 0)
				leds[XY(x, y)] = CHSV(random(0, 255), 255, 255);
		}
		fader(70);
	}

private:
	void fader(byte step)
	{
		for (byte i = 0; i < matrixWidth; i++)
		{
			for (byte j = 0; j < matrixWidth; j++)
			{
				fadePixel(i, j, step);
			}
		}
	}
	void fadePixel(byte i, byte j, byte step)
	{

		uint16_t pixelNum = XY(i, j);
		if (colorIndex(pixelNum) == 0)
			return;

		if (leds[pixelNum].r >= 30 ||
			leds[pixelNum].g >= 30 ||
			leds[pixelNum].b >= 30)
		{
			leds[pixelNum].fadeToBlackBy(step);
		}
		else
		{
			leds[pixelNum] = 0;
		}
	}
};

class RainbowVertical : public Effect
{
	byte hue;

public:
	RainbowVertical() : Effect()
	{
		m_scale = 20;
	}

	void update(uint32_t tick) override
	{
		hue += 2;
		for (byte j = 0; j < matrixHeight; j++)
		{
			CHSV thisColor = CHSV((byte)(hue + j * m_scale), 255, 255);
			for (byte i = 0; i < matrixWidth; i++)
				leds[XYsafe(i, j)] = thisColor;
		}
	}
};

#endif