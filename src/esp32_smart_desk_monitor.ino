#include <WiFi.h>
#include <Wire.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Use local credentials for development. Do not commit real Wi-Fi credentials.
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define DHTPIN 4
#define DHTTYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// Fixed India Standard Time offset (UTC+05:30; no daylight-saving adjustment).
constexpr long GMT_OFFSET_SEC = 19800;
constexpr int DAYLIGHT_OFFSET_SEC = 0;

constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 250;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 15000;
constexpr unsigned long TIME_CHECK_INTERVAL_MS = 1000;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

float temperatureC = NAN;
float humidity = NAN;

unsigned long lastSensorRead = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastWiFiAttempt = 0;
unsigned long lastTimeCheck = 0;

bool wifiStarted = false;
bool ntpConfigured = false;
bool timeSynced = false;

void startWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  wifiStarted = true;
  lastWiFiAttempt = millis();
  Serial.println("Starting Wi-Fi connection...");
}

void maintainNetworkTime() {
  const unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    // If Wi-Fi drops, reconfigure SNTP after the next successful connection.
    ntpConfigured = false;

    if (!wifiStarted) {
      startWiFi();
    } else if (now - lastWiFiAttempt >= WIFI_RETRY_INTERVAL_MS) {
      Serial.println("Wi-Fi is disconnected; retrying...");
      WiFi.reconnect();
      lastWiFiAttempt = now;
    }
    return;
  }

  if (!ntpConfigured) {
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC,
               "pool.ntp.org", "time.nist.gov");
    ntpConfigured = true;
    Serial.print("Wi-Fi connected. IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("SNTP time synchronization requested.");
  }

  // A zero timeout prevents the display loop from blocking while time is unavailable.
  if (!timeSynced && now - lastTimeCheck >= TIME_CHECK_INTERVAL_MS) {
    lastTimeCheck = now;
    struct tm timeInfo;
    if (getLocalTime(&timeInfo, 0) && (timeInfo.tm_year + 1900 >= 2024)) {
      timeSynced = true;
      Serial.println("System time synchronized.");
    }
  }
}

void readSensor() {
  const float newHumidity = dht.readHumidity();
  const float newTemperature = dht.readTemperature();

  // Retain the last valid value if a sensor read fails.
  if (!isnan(newTemperature)) temperatureC = newTemperature;
  if (!isnan(newHumidity)) humidity = newHumidity;
}

void drawDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  struct tm timeInfo;
  if (timeSynced && getLocalTime(&timeInfo, 0)) {
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
  display.println("Starting...");
  display.display();

  startWiFi();
  readSensor();
  lastSensorRead = millis();
  drawDisplay();
  lastDisplayUpdate = millis();
}

void loop() {
  const unsigned long now = millis();

  maintainNetworkTime();

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;
    readSensor();
  }

  if (now - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    lastDisplayUpdate = now;
    drawDisplay();
  }
}
