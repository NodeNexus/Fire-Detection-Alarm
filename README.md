# ESP32 Multi-Sensor Fire Detection & Alarm System

## 🚀 Features

* **Flame Detection:** Uses an **LM393** IR sensor for instant, direct flame detection.
* **Smoke & Gas Detection:** Monitors for combustible gases and smoke using an **MQ-2** sensor.
* **Temperature Monitoring:** A **DHT22** sensor tracks ambient temperature, acting as a secondary check for high-heat conditions.
* **Visual Display:** A 0.96" **SSD1306** OLED screen shows real-time sensor data and system status ("SYSTEM NORMAL" / "FIRE ALERT").
* **Audible & Visual Alarm:** A bright **Red LED** and a loud **Buzzer** immediately activate when a fire condition is detected.
* **Dual-Trigger Logic:** The alarm triggers if **either** a flame is seen (digital trigger) **or** smoke levels exceed a calibrated threshold (analog trigger).

---

## 📦 Bill of Materials (Components)

* 1x ESP32 Development Board
* 1x LM393 Flame Sensor Module
* 1x MQ-2 Gas/Smoke Sensor Module
* 1x DHT22 Temperature & Humidity Sensor
* 1x SSD1306 0.96" I2C OLED Display
* 1x Red LED (5mm)
* 1x Active Buzzer
* 1x 220Ω Resistor (for the LED)
* 1x Breadboard
* Jumper Wires

---

## 🔌 Wiring Diagram

**Power Rails:**
* Connect the ESP32 **`VIN`** pin to the **5V rail** of the breadboard.
* Connect the ESP32 **`3V3`** pin to the **3.3V rail**.
* Connect the ESP32 **`GND`** pin to the **GND rail**.

| Component | Component Pin | Connection | Notes |
| :--- | :--- | :--- | :--- |
| **SSD1306 OLED**| `VCC` | `3V3` (3.3V Rail) | |
| | `GND` | `GND` (GND Rail) | |
| | `SCL` | `GPIO 22` | I2C Clock |
| | `SDA` | `GPIO 21` | I2C Data |
| | | | |
| **DHT22** | `VCC` / `+` | `3V3` (3.3V Rail) | |
| | `GND` / `-` | `GND` (GND Rail) | |
| | `DATA` | `GPIO 4` | (Add a 10kΩ pull-up resistor from this pin to 3.3V) |
| | | | |
| **MQ-2 Sensor** | `VCC` | `VIN` (5V Rail) | Runs on 5V |
| | `GND` | `GND` (GND Rail) | |
| | `A0` | `GPIO 34` | Analog Input |
| | | | |
| **LM393 Sensor**| `VCC` | `VIN` (5V Rail) | Runs on 5V (or 3.3V) |
| | `GND` | `GND` (GND Rail) | |
| | `D0` | `GPIO 5` | Digital Input |
| | | | |
| **Red LED** | Anode (+) | 220Ω Resistor | Other end of resistor to `GPIO 26` |
| | Cathode (-) | `GND` (GND Rail) | |
| | | | |
| **Buzzer** | `+` pin | `GPIO 25` | |
| | `-` pin | `GND` (GND Rail) | |

---

## ⚙️ Software & Setup

1.  **Install Arduino IDE:** Make sure you have the latest [Arduino IDE](https://www.arduino.cc/en/software) installed.
2.  **Add ESP32 Board Manager:**
    * Go to **File > Preferences**.
    * In "Additional Board Manager URLs", add:
        ```
        [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
        ```
    * Go to **Tools > Board > Boards Manager...**, search for "esp32", and install it.
3.  **Install Libraries:**
    * Go to **Sketch > Include Library > Manage Libraries...**.
    * Search for and install the following libraries:
        * `Adafruit SSD1306`
        * `Adafruit GFX Library`
        * `DHT sensor library` (by Adafruit)
        * `Adafruit Unified Sensor` (will be installed as a dependency)

---

## 🔥 Usage & Calibration

### **Important: Calibrating the Smoke Sensor**

The `SMOKE_THRESHOLD` value in the code **must** be calibrated for your specific sensor and environment to prevent false alarms.

1.  Upload the code and open the Arduino Serial Monitor at **115200 baud**.
2.  Let the MQ-2 sensor warm up for 1-2 minutes. You will see the "Smoke Level" value in the Serial Monitor.
3.  Note the stable "Smoke Level" value in clean air (e.g., `450`).
4.  Safely introduce a small amount of smoke (e.g., from a blown-out match) near the sensor. Note the new, higher value (e.g., `1500`).
5.  In the code, set your `SMOKE_THRESHOLD` to a value between these two (e.g., `1000`).

### System Status

* **SYSTEM NORMAL:** The OLED will display "SYSTEM NORMAL" and show the live Temperature, Humidity, and Smoke Level readings. The LED and Buzzer will be OFF.
* **FIRE ALERT:** If a flame is detected (from the LM393) OR the smoke level (from the MQ-2) goes above your `SMOKE_THRESHOLD`, the system will enter an alert state:
    * The OLED screen will flash "FIRE ALERT!".
    * The Red LED will turn ON.
    * The Buzzer will turn ON.

---
## 👨‍💻 Developer
This project was developed by Vaishnav Balpande (NodeNexus).
