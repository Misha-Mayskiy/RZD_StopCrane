#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h> // для OLED дисплея

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // Размеры экрана
#define PRESSURE_SENSOR_PIN A0  // Пин для датчика давления

// Пороговое значение давления
const float PRESSURE_THRESHOLD = 5.0;

// Настройки Wi-Fi
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const IPAddress serverIP(192, 168, 1, 100); // IP адрес главной платы
const uint16_t serverPort = 5000;

// Настройки дисплея
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Инициализация Wi-Fi клиента
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Инициализация дисплея
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Не удалось инициализировать дисплей!"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi подключен.");
  displayMessage("WiFi подключен.");
}

void loop() {
  float pressure = readPressure();

  // Передача данных на главную плату по Wi-Fi
  if (client.connect(serverIP, serverPort)) {
    client.print("Давление: ");
    client.println(pressure);
    client.stop();
  }

  // Вывод давления в консоль
  Serial.print("[");
  Serial.print(millis() / 1000);
  Serial.print(" с] Текущее давление: ");
  Serial.print(pressure);
  Serial.println(" атм.");

  // Проверка давления и вывод на дисплей в случае неполадки
  if (pressure < PRESSURE_THRESHOLD) {
    displayMessage("ВНИМАНИЕ: Падение давления!");
    Serial.print("[");
    Serial.print(millis() / 1000);
    Serial.print(" с] Проблема: давление ");
    Serial.print(pressure);
    Serial.println(" атмосфер!");
  }

  delay(1000); // Проверка каждую секунду
}

// Чтение данных с датчика давления
float readPressure() {
  int sensorValue = analogRead(PRESSURE_SENSOR_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  float pressure = voltage * 10.0; // Конвертация в атмосферное давление
  return pressure;
}

// Вывод сообщения на дисплей
void displayMessage(String message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}
