#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> // Для работы с веб-сервером
#include <FS.h>               // Для работы с файловой системой (LittleFS)

// --- Настройки Wi-Fi ---
const char* ssid = "PLAY_Swiatlowod_D9DB";   
const char* password = "Qs@#7mrGmnPG"; 

// --- Настройки светодиодов ---
#define LED_PIN     D2        // Пин, к которому подключены данные светодиодной ленты (например, D2 на NodeMCU)
#define NUM_LEDS    256       // Количество светодиодов в матрице 16x16 (16 * 16 = 256)
#define BRIGHTNESS  64        // Начальная яркость (от 0 до 255)
#define COLOR_ORDER GRB       // Порядок цветов (GRB для большинства матриц)
#define LED_TYPE    WS2812B   // Тип светодиодов (например, WS2812B)

// Определяем массив для хранения информации о каждом светодиоде
CRGB leds[NUM_LEDS];

// --- Глобальные переменные для режимов ---
uint8_t currentBrightness = BRIGHTNESS; // Текущая яркость
uint8_t currentMode = 0;              // Текущий режим (индекс)
uint8_t currentSpeed = 50;            // Текущая скорость (от 0 до 100, где 100 - самая быстрая)

// --- Объявления функций для режимов ---
void modeSolidColor();
void modeRainbowChase();

// Массив указателей на функции режимов
typedef void (*LampMode)(); // Определяем тип указателя на функцию без аргументов и возвращаемого значения
LampMode lampModes[] = {
    modeSolidColor,
    modeRainbowChase
};
const uint8_t totalModes = sizeof(lampModes) / sizeof(lampModes[0]); // Количество доступных режимов

// --- Объект веб-сервера ---
ESP8266WebServer server(80); // Создаем объект сервера на порту 80 (стандартный HTTP)

// --- Обработчики запросов веб-сервера ---

// Обработка корневого запроса "/"
void handleRoot() {
  Serial.println("Handling root request '/'");
  // Здесь мы будем отдавать HTML-страницу
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>GyverLamp Web</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #222; color: #eee; }";
  html += "h1 { color: #00bcd4; }";
  html += "button { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; margin: 5px; }";
  html += "button:hover { background-color: #45a049; }";
  html += ".slider-container { margin: 20px 0; }";
  html += "input[type='range'] { width: 80%; max-width: 400px; height: 10px; background: #d3d3d3; outline: none; opacity: 0.7; transition: opacity .2s; border-radius: 5px; }";
  html += "input[type='range']::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 20px; height: 20px; border-radius: 50%; background: #00bcd4; cursor: pointer; }";
  html += "input[type='range']::-moz-range-thumb { width: 20px; height: 20px; border-radius: 50%; background: #00bcd4; cursor: pointer; }";
  html += "</style></head><body>";
  html += "<h1>GyverLamp Web Control</h1>";
  html += "<div>";
  html += "<p>Brightness: <span id='brightnessValue'>" + String(currentBrightness) + "</span></p>";
  html += "<div class='slider-container'><input type='range' id='brightnessSlider' min='0' max='255' value='" + String(currentBrightness) + "' onchange='setBrightness(this.value)'></div>";
  html += "<p>Speed: <span id='speedValue'>" + String(currentSpeed) + "</span></p>";
  html += "<div class='slider-container'><input type='range' id='speedSlider' min='0' max='100' value='" + String(currentSpeed) + "' onchange='setSpeed(this.value)'></div>";
  html += "</div>";
  html += "<div>";
  html += "<p>Current Mode: <span id='modeValue'>" + String(currentMode) + "</span></p>";
  html += "<button onclick='changeMode(0)'>Mode 0 (Solid Color)</button>";
  html += "<button onclick='changeMode(1)'>Mode 1 (Rainbow Chase)</button>";
  // Можно добавить больше кнопок для режимов, когда их будет больше
  html += "</div>";
  html += "<script>";
  html += "function setBrightness(value) {";
  html += "  document.getElementById('brightnessValue').innerText = value;";
  html += "  fetch('/brightness?value=' + value);";
  html += "}";
  html += "function setSpeed(value) {";
  html += "  document.getElementById('speedValue').innerText = value;";
  html += "  fetch('/speed?value=' + value);";
  html += "}";
  html += "function changeMode(mode) {";
  html += "  document.getElementById('modeValue').innerText = mode;";
  html += "  fetch('/mode?value=' + mode);";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Обработка запроса изменения яркости
void handleBrightness() {
  if (server.hasArg("value")) {
    currentBrightness = server.arg("value").toInt(); // Просто обновляем переменную
    // FastLED.setBrightness(currentBrightness); // Эту строку УДАЛИМ
    Serial.printf("Brightness set to: %d\n", currentBrightness);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// Обработка запроса изменения скорости
void handleSpeed() {
  if (server.hasArg("value")) {
    currentSpeed = server.arg("value").toInt();
    Serial.printf("Speed set to: %d\n", currentSpeed);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// Обработка запроса изменения режима
void handleMode() {
  if (server.hasArg("value")) {
    uint8_t newMode = server.arg("value").toInt();
    if (newMode < totalModes) { // Проверяем, что режим существует
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

// Обработчик для несуществующих страниц
void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting GyverLampWeb...");

  // Инициализация FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(currentBrightness);
  FastLED.show();

  // --- Подключение к Wi-Fi ---
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // --- Настройка веб-сервера ---
  server.on("/", handleRoot); // Когда запрашивают корень "/", вызываем handleRoot
  server.on("/brightness", handleBrightness); // Обработчик для /brightness
  server.on("/speed", handleSpeed);         // Обработчик для /speed
  server.on("/mode", handleMode);           // Обработчик для /mode
  server.onNotFound(handleNotFound);        // Обработчик для несуществующих страниц

  server.begin(); // Запускаем веб-сервер
  Serial.println("HTTP server started");
}

void loop() {
  // Устанавливаем яркость перед отрисовкой каждого кадра
  FastLED.setBrightness(currentBrightness); // Эту строку ДОБАВИМ

  // Вызываем текущий режим
  lampModes[currentMode]();
  FastLED.show(); // Обновляем светодиоды после выполнения режима
  FastLED.delay(1000 / 60); // Ограничиваем FPS для плавности и экономии ресурсов

  server.handleClient(); // Обязательно вызываем это для обработки входящих запросов
}

// --- Реализация режимов ---

// Режим 0: Фиксированный цвет (красный для примера)
void modeSolidColor() {
  fill_solid(leds, NUM_LEDS, CRGB::Red); // Всегда красный
}

// Режим 1: Бегущая радуга
void modeRainbowChase() {
  // Скорость влияет на смещение цвета.
  // Делим на 10.0, чтобы получить значение 0-10, которое затем умножается на millis()
  // Чем выше currentSpeed, тем быстрее меняется радуга.
  // (100 - currentSpeed) + 1: инвертируем скорость, чтобы 0 была самая быстрая, 100 самая медленная
  // или просто currentSpeed / 10.0 для прямой зависимости
  uint8_t hue = (millis() / ((100 - currentSpeed) + 1)); // Более низкое значение для (100-speed)+1 = более быстрые изменения
  fill_rainbow(leds, NUM_LEDS, hue, 5); // 5 - шаг изменения оттенка между светодиодами
}