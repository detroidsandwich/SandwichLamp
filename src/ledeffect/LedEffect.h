#ifndef LED_EFFECT_H
#define LED_EFFECT_H

#include "LedSettings.h"

const uint8_t matrixWidth = MATRIX_WIDTH; // change to led settings
const uint8_t matrixHeight = MATRIX_HEIGHT;

// Abstract class for implement
class LedEffect
{
public:
	LedEffect(const String &name) { this->name = name; }

	virtual ~LedEffect() {}

	inline virtual void update(uint32_t tick) { Serial.println("not implement"); }

	virtual void setSpeed(uint8_t speed) { m_speed = speed; }
	uint8_t getSpeed() { return m_speed; }

	virtual void setScale(uint8_t scale) { m_scale = scale; }
	uint8_t getScale() { return m_scale; }

	String getName() { return name; }

protected:
	String name;
	uint8_t m_speed = 64; // default speed 0..255
	uint8_t m_scale = 64; // default scale 0..255
};

// Effect for FastLed matrix
//
// RotateRainbow ------------------------
//
class RotateRainbow : public LedEffect
{
public:
	RotateRainbow() : LedEffect("RotateRainbow") {}

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
//
// SparklesRoutine ------------------------
//
class SparklesRoutine : public LedEffect
{
	uint32_t prevUpdateMillis = 0;

public:
	SparklesRoutine() : LedEffect("SparklesRoutine") {}

	void update(uint32_t tick) override
	{
		if (tick - prevUpdateMillis > (255 - m_scale))
		{
			prevUpdateMillis = tick;
			for (byte i = 0; i < (m_scale / 32) + 1; i++)
			{
				byte x = random(0, matrixWidth);
				byte y = random(0, matrixHeight);
				if (colorXY(x, y) == CRGB::Black)
					leds[XY(x, y)] = CHSV(random(0, 255), 255, 255);
			}
		}
		fader((m_speed + 16) / 8);
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
//
// RainbowVertical ------------------------
//
class RainbowVertical : public LedEffect
{
	byte hue;

public:
	RainbowVertical() : LedEffect("RainbowVertical") {}

	void update(uint32_t tick) override
	{
		hue += (m_speed / 16);
		byte factor = (m_scale / 4);
		for (byte j = 0; j < matrixHeight; j++)
		{
			CHSV thisColor = CHSV((hue + j * factor), 255, 255);
			for (byte i = 0; i < matrixWidth; i++)
				leds[XYsafe(i, j)] = thisColor;
		}
	}
};
//
// Matrix ------------------------
//
class Matrix : public LedEffect
{
	uint32_t prevUpdateMillis = 0;

public:
	Matrix() : LedEffect("Matrix") {}

	void update(uint32_t tick) override
	{
		if (tick - prevUpdateMillis < (255 - m_speed))
		{
			return;
		}
		prevUpdateMillis = tick;
		CHSV thisColor = CHSV(HUE_GREEN, 255, 255); // HUE_GREEN for green
		for (byte y = 0; y < matrixHeight; y++)
		{
			for (byte x = 0; x < matrixWidth; x++)
			{
				uint32_t currentColor = colorXY(x, y);
				if (currentColor == thisColor)
				{
					leds[XYsafe(x, y - 1)] = thisColor;
					leds[XYsafe(x, y)].fadeToBlackBy(m_scale / 8);
				}
				else if (currentColor != CRGB::Black)
				{
					leds[XYsafe(x, y)].fadeToBlackBy(m_scale / 4);
				}
			}
		}

		for (byte i = 0; i < ((255 - m_scale) / matrixHeight); i++)
		{
			long x = random(0, matrixWidth);
			if (colorXY(x, matrixHeight - 1) == CRGB::Black)
			{
				leds[XYsafe(x, matrixHeight - 1)] = thisColor;
			}
		}
	}
};

class ColorBlink : public LedEffect
{
	uint32_t prevUpdateMillis = 0;

public:
	ColorBlink() : LedEffect("ColorBlink") {}

	void update(uint32_t tick) override
	{
		if (tick - prevUpdateMillis < (255 - m_speed))
		{
			return;
		}
		prevUpdateMillis = tick;

		// Генерация нового цвета
		CRGB newColor = CHSV(random8(), 255, 255); // Новый цвет в HSV формате

		// Плавное изменение цвета на LED-матрице
		for (int i = 0; i < matrixWidth * matrixHeight; i++)
		{
			nblend(leds[i], newColor, 32); // Плавное смешивание цветов с коэффициентом 32
		}
	}
};

class WhiteEffect : public LedEffect
{
	
public:
	WhiteEffect() : LedEffect("WhiteEffect") {}

	void update(uint32_t tick) override
	{
		// hue += (m_speed / 16);
		// byte factor = (m_scale / 4);
		for (byte j = 0; j < matrixHeight; j++)
		{
			for (byte i = 0; i < matrixWidth; i++)
				leds[XYsafe(i, j)] = CRGB::White;
		}
	}
};
#endif