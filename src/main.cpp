#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h> // Для работы с файловой системой LittleFS
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include "secrets.h"

// --- Имя хоста для mDNS ---
const char* host = "sandwichlamp"; // Твоя лампа будет доступна по адресу http://sandwichlamp.local

// --- Настройки светодиодов ---
#define LED_PIN     D2
#define NUM_LEDS    256
#define BRIGHTNESS  64
#define COLOR_ORDER GRB
#define LED_TYPE    WS2812B

CRGB leds[NUM_LEDS];

// --- Глобальные переменные для режимов ---
uint8_t currentBrightness = BRIGHTNESS;
uint8_t currentMode = 0;
uint8_t currentSpeed = 50; // Скорость от 0 (самая медленная) до 100 (самая быстрая)

// --- Переменные для неблокирующей задержки ---
unsigned long previousMillis = 0;
// Частота обновления FastLED в миллисекундах.
// 1000мс / 60 FPS = ~16мс
const long frameDelayMillis = 1000 / 60; // Задержка между кадрами (16 мс для 60 FPS)

// --- Объявления функций для режимов ---
void modeSolidColor();
void modeRainbowChase();
// Добавляем новый режим для демонстрации легкости добавления
void modeFire();

// Массив указателей на функции режимов
typedef void (*LampMode)();
LampMode lampModes[] = {
    modeSolidColor,
    modeRainbowChase,
    modeFire // Добавляем новый режим
};
const uint8_t totalModes = sizeof(lampModes) / sizeof(lampModes[0]);

// --- Объект веб-сервера ---
ESP8266WebServer server(80);

// --- Обработчики запросов веб-сервера (пока остаются в C++) ---
// Примечание: handleRoot будет изменена для отдачи файла из LittleFS

void handleRoot() {
  Serial.println("Handling root request '/'");
  // Попытка отдать файл index.html из LittleFS
  if (LittleFS.exists("/index.html")) {
    File file = LittleFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
  } else {
    server.send(404, "text/plain", "index.html not found on LittleFS");
  }
}

void handleBrightness() {
  if (server.hasArg("value")) {
    currentBrightness = server.arg("value").toInt();
    Serial.printf("Brightness set to: %d\n", currentBrightness);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSpeed() {
  if (server.hasArg("value")) {
    currentSpeed = server.arg("value").toInt();
    Serial.printf("Speed set to: %d\n", currentSpeed);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleMode() {
  if (server.hasArg("value")) {
    uint8_t newMode = server.arg("value").toInt();
    if (newMode < totalModes) {
      currentMode = newMode;
      Serial.printf("Mode set to: %d\n", currentMode);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(404, "text/plain", "Mode Not Found");
    }
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// Обработчик для статических файлов (CSS, JS)
void handleStaticFile() {
  String path = server.uri();
  if (LittleFS.exists(path)) {
    String contentType = "text/plain"; // По умолчанию
    if (path.endsWith(".html")) contentType = "text/html";
    else if (path.endsWith(".css")) contentType = "text/css";
    else if (path.endsWith(".js")) contentType = "application/javascript";
    else if (path.endsWith(".ico")) contentType = "image/x-icon"; // Для favicon
    else if (path.endsWith(".gz")) contentType = "application/x-gzip"; // Если будем использовать сжатие
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "File Not Found");
  }
}

void handleNotFound() {
  Serial.printf("File not found: %s\n", server.uri().c_str());
  server.send(404, "text/plain", "Not Found");
}

// Обработчик для запроса текущего состояния
void handleGetState() {
  Serial.println("Handling /state request");
  DynamicJsonDocument doc(256); // Создаем JSON-документ (размер может быть больше, если будет больше параметров)
  doc["brightness"] = currentBrightness;
  doc["speed"] = currentSpeed;
  doc["mode"] = currentMode;

  String jsonString;
  serializeJson(doc, jsonString); // Преобразуем JSON-документ в строку

  server.send(200, "application/json", jsonString); // Отправляем JSON-ответ
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting GyverLampWeb...");

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  // ... (FastLED initialization) ...

  // --- Подключение к Wi-Fi ---
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID); // <<<--- Используем WIFI_SSID из secrets.h

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // --- Инициализация mDNS ---
  if (!MDNS.begin(host)) { // Запускаем mDNS с именем хоста "gyverlamp"
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("MDNS responder started");
    Serial.printf("Access your lamp at http://%s.local\n", host);
  }

  // --- Настройка веб-сервера ---
  server.on("/", handleRoot);
  server.on("/brightness", handleBrightness);
  server.on("/speed", handleSpeed);
  server.on("/mode", handleMode);
  server.on("/state", handleGetState);
  server.onNotFound(handleStaticFile);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= frameDelayMillis) {
    previousMillis = currentMillis;

    FastLED.setBrightness(currentBrightness);
    lampModes[currentMode]();
    FastLED.show();
  }

  server.handleClient();
  MDNS.update(); // Обязательно вызываем MDNS.update() в loop()
}

// --- Реализация режимов ---

// Режим 0: Фиксированный цвет (красный для примера)
void modeSolidColor() {
  fill_solid(leds, NUM_LEDS, CRGB::Red);
}

// Режим 1: Бегущая радуга
void modeRainbowChase() {
  // Скорость влияет на смещение цвета.
  // currentSpeed от 0 (медленно) до 100 (быстро)
  // map(currentSpeed, 0, 100, 10, 1) - инвертируем скорость для делителя
  // Если currentSpeed=0, делитель = 10 (медленно). Если currentSpeed=100, делитель = 1 (быстро).
  uint8_t speedDivisor = map(currentSpeed, 0, 100, 20, 1); // Меньшее число = быстрее
  uint8_t hue = (millis() / speedDivisor);
  fill_rainbow(leds, NUM_LEDS, hue, 5);
}

// Режим 2: Огонь (новый режим)
void modeFire() {
  // Простой эффект огня
  static byte heat[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((255 * 10) / NUM_LEDS) + 1));
  }
  for (int j = 0; j < NUM_LEDS; j++) {
    heat[j] = qadd8(heat[j], random8(0, 5));
  }
  for (int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = HeatColor(heat[i]);
  }
  // Скорость в режиме огня может влиять на "затухание" или "разгорание"
  // Но для простоты пока не привязана к currentSpeed
}