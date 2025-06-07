#include <Esp.h>
#include <Arduino.h>
#include <FastLED.h>

// --- Настройки светодиодов ---
#define LED_PIN     D2        // Пин, к которому подключены данные светодиодной ленты (например, D2 на NodeMCU)
#define NUM_LEDS    256       // Количество светодиодов в матрице 16x16 (16 * 16 = 256)
#define BRIGHTNESS  64        // Начальная яркость (от 0 до 255)
#define COLOR_ORDER GRB       // Порядок цветов (GRB для большинства матриц)
#define LED_TYPE    WS2812B   // Тип светодиодов (например, WS2812B)

// Определяем массив для хранения информации о каждом светодиоде
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200); // Инициализация последовательного порта для отладки
  Serial.println("Starting GyverLampWeb...");

  // Инициализация FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Устанавливаем все светодиоды в красный цвет для теста
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show(); // Отправляем данные на светодиоды
}

void loop() {
  // В loop пока ничего не делаем, просто держим светодиоды красными
}