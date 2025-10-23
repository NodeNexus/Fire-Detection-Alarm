/*
 * ======================================================
 * Project: ESP32 Fire Detection System
 * Author:  NodeNexus
 * GitHub:  https://github.com/NodeNexus
 *
 * Description:
 * This system monitors for fire using a flame sensor (LM393),
 * a smoke/gas sensor (MQ-2), and a temperature sensor (DHT22).
 * All data and alerts are displayed on an SSD1306 OLED screen
 * and trigger a red LED and a buzzer.
 * ======================================================
 */

// --- LIBRARIES ---
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin Definitions ---
#define DHT_PIN         14     // DHT22 Data Pin
#define FLAME_PIN       27     // LM393 Flame Sensor Digital Out (D0)
#define MQ_PIN          26    // MQ-2 Gas/Smoke Sensor Analog Out (A0)
#define OLED_SCL        22    // OLED SCL Pin
#define OLED_SDA        21    // OLED SDA Pin

// --- NEW ALARM PINS ---
#define BUZZER_PIN      25    // Active Buzzer (+) pin
#define LED_PIN         26    // Red LED Anode (+) pin via 220-ohm resistor

// --- OLED Display Settings ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- DHT Sensor Settings ---
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// --- Alert Threshold ---
int SMOKE_THRESHOLD = 1000; // Calibrate this value!

// --- Global Variables ---
float tempC;
float humidity;
int smokeValue;
int flameState; // 0 = Flame Detected, 1 = No Flame

void setup() {
  Serial.begin(115200);

  // Initialize Sensors
  pinMode(FLAME_PIN, INPUT);
  dht.begin();

  // --- NEW ---
  // Initialize Alarm Outputs
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // Make sure they are off at the start
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  // --- END NEW ---

  // Initialize OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Fire Detection Sys"));
  display.println(F("Initializing..."));
  display.display();
  delay(2000);
}

void loop() {
  // 1. Read all sensor data
  humidity = dht.readHumidity();
  tempC = dht.readTemperature();
  smokeValue = analogRead(MQ_PIN);
  flameState = digitalRead(FLAME_PIN); // Reads 0 (LOW) if flame is detected

  // Print to serial monitor for debugging
  Serial.printf("Temp: %.1f C, Hum: %.1f %%, Smoke: %d, Flame: %d\n",
                tempC, humidity, smokeValue, flameState);

  // 2. Check for emergency conditions
  if (flameState == LOW || smokeValue > SMOKE_THRESHOLD) {
    displayFireAlert();
    // Activate alarms
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    displayNormalStatus();
    // Deactivate alarms
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  delay(1000); // Wait 1 second before next read
}

void displayNormalStatus() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE); // Ensure text is white
  display.println(F("SYSTEM"));
  display.println(F("NORMAL"));
  
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.printf("T:%.1fC H:%.1f%%\n", tempC, humidity);
  display.setCursor(0, 50);
  display.printf("Smoke Level: %d", smokeValue);
  
  display.display();
}

void displayFireAlert() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.fillScreen(SSD1306_WHITE); // Invert screen
  display.setTextColor(SSD1306_BLACK);
  
  display.println(F(" FIRE!!"));
  display.println(F(" ALERT!"));

  display.setTextSize(1);
  if (flameState == LOW) {
    display.println(F("!! FLAME DETECTED !!"));
  }
  if (smokeValue > SMOKE_THRESHOLD) {
    display.println(F("!! SMOKE DETECTED !!"));
  }
  
  display.display();
}