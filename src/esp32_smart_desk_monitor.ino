#include <WiFi.h>
#include <Wire.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define DHTPIN 4
#define DHTTYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

const long GMT_OFFSET_SEC = 19800;
const int DAYLIGHT_OFFSET_SEC = 0;

const unsigned long SENSOR_INTERVAL_MS = 2000;
const unsigned long DISPLAY_INTERVAL_MS = 250;
const unsigned long NTP_RESYNC_INTERVAL_MS = 6UL * 60UL * 60UL * 1000UL;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

float temperatureC = NAN;
float humidity = NAN;
unsigned long lastSensorRead = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastNtpSync = 0;

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(250);
  }
}

void syncTime() {
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org", "time.nist.gov");

  struct tm timeInfo;
  unsigned long start = millis();
  while (!getLocalTime(&timeInfo) && millis() - start < 10000) {
    delay(250);
  }

  lastNtpSync = millis();
}

void readSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(t)) temperatureC = t;
  if (!isnan(h)) humidity = h;
}

void drawDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    char timeBuffer[9];
    char dateBuffer[11];

    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);
    strftime(dateBuffer, sizeof(dateBuffer), "%d-%m-%Y", &timeInfo);

    display.setTextSize(2);
    display.setCursor(8, 2);
    display.println(timeBuffer);

    display.setTextSize(1);
    display.setCursor(36, 23);
    display.println(dateBuffer);
  } else {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Time: syncing...");
  }

  display.setTextSize(1);
  display.setCursor(0, 37);
  display.print("Temp: ");
  if (isnan(temperatureC)) display.print("--");
  else display.print(temperatureC, 1);
  display.println(" C");

  display.setCursor(0, 51);
  display.print("Humidity: ");
  if (isnan(humidity)) display.print("--");
  else display.print(humidity, 1);
  display.println(" %");

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED initialization failed.");
    while (true) delay(1000);
  }

  dht.begin();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Smart Desk Monitor");
  display.println();
  display.println("Connecting Wi-Fi...");
  display.display();

  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi connected. IP: ");
    Serial.println(WiFi.localIP());
    syncTime();
  } else {
    Serial.println("Wi-Fi connection failed. Temperature/humidity will still work.");
  }

  readSensor();
  drawDisplay();
}

void loop() {
  const unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    if (WiFi.status() == WL_CONNECTED) syncTime();
  }

  if (WiFi.status() == WL_CONNECTED && now - lastNtpSync >= NTP_RESYNC_INTERVAL_MS) {
    syncTime();
  }

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;
    readSensor();
  }

  if (now - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    lastDisplayUpdate = now;
    drawDisplay();
  }
}
