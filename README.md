# ESP32 Mini Smart Desk Monitor

A compact ESP32 desk display that combines a Wi-Fi-synchronized clock with live temperature and humidity readings on an OLED screen.

## Problem solved

A normal desk clock only shows the time, while checking temperature and humidity usually requires a separate thermometer/hygrometer or opening an app. This project combines those everyday readings into one small, USB-powered device that can sit on a desk, workstation, gaming setup, or dashboard.

## Features

- Wi-Fi-synchronized clock using NTP
- Live temperature monitoring
- Live humidity monitoring
- 128×64 I2C OLED display
- USB powered
- Custom handmade metal frame
- Simple firmware that can be expanded with more sensors and features

## Hardware

- ESP32 development board
- 0.96-inch SSD1306 128×64 I2C OLED
- DHT11 or DHT22 temperature/humidity sensor
- Jumper wires
- USB cable/power source
- Custom metal frame/enclosure

## Wiring

### OLED (SSD1306 I2C)

| OLED | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### DHT sensor

| DHT | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO 4 |

Many DHT sensor modules already include the required pull-up resistor. For a bare sensor, use the resistor recommended by its datasheet.

## Software setup

### Arduino IDE

Install the ESP32 board package and these libraries:

- Adafruit SSD1306
- Adafruit GFX Library
- DHT sensor library by Adafruit
- Adafruit Unified Sensor

Open `src/esp32_smart_desk_monitor.ino`, enter your Wi-Fi credentials, select your ESP32 board, and upload.

### PlatformIO

This repository includes `platformio.ini`. Open the project in PlatformIO and build/upload normally.

## Configuration

At the top of the sketch:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
```

Default settings:

- OLED address: `0x3C`
- I2C SDA: GPIO 21
- I2C SCL: GPIO 22
- DHT data: GPIO 4
- DHT type: DHT22
- Time zone: Asia/Kolkata (UTC+5:30)

For a DHT11, change:

```cpp
#define DHTTYPE DHT22
```

to:

```cpp
#define DHTTYPE DHT11
```

## How it works

1. The ESP32 connects to Wi-Fi.
2. It synchronizes local time from an NTP server.
3. The DHT sensor provides temperature and humidity readings.
4. The ESP32 refreshes the OLED with the current time, date, temperature, and humidity.
5. If a sensor read fails, the last valid reading is retained.

## Project structure

```text
.
├── README.md
├── LICENSE
├── platformio.ini
└── src/
    └── esp32_smart_desk_monitor.ino
```

## Future ideas

- Weather information
- Air-quality sensor
- Automatic display brightness
- Alarm/timer
- Minimum/maximum temperature history
- CO₂ monitoring
- Room-status icons
- Improved enclosure

## Status

Latest prototype. The enclosure and additional features are still being improved.

## License

MIT License. See [LICENSE](LICENSE).
