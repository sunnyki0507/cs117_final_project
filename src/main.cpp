#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_CAP1188.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "DHT20.h"
#include <ArduinoJson.h>
#include <string>

// =======================
// Hardware config (LED, LDR, CAP1188)
// =======================

bool ledEnabled = true;
bool wasTouched = false;

const int LED_PIN = 33;
const int LDR_PIN = 35;

static const float VREF = 3.3f;
static const int MAXADC = 4095;

const int LEDC_CH   = 0;
const int LEDC_FREQ = 5000;
const int LEDC_RES  = 12;
const int MIN_DUTY  = 60;

#define CAP1188_RESET 9 
#define CAP1188_CS    10


#define SDA_PIN 21
#define SCL_PIN 22
int minLight = 4095;
int maxLight = 0;

Adafruit_CAP1188 cap = Adafruit_CAP1188();


int     lastLdrValue      = 0;
uint8_t lastTouchCount    = 0;
float energyBaseline_s = 0.0f;   // "full-brightness seconds"
float energyActual_s   = 0.0f;   // "actual-brightness seconds"
uint32_t lastEnergyUpdate = 0;

// =======================
// Wi-Fi + Azure IoT config
// =======================

#define WIFI_SSID     "UCInet Mobile Access"
#define WIFI_PASSWORD ""

// Azure IoT Hub configuration
#define SAS_TOKEN "SharedAccessSignature sr=cs147-hub-10.azure-devices.net%2Fdevices%2Fcs147_project&sig=WXH2LPjvNS6ZBKqyHrobw0upIiRJyEvnwOKdWQ8bYsg%3D&se=1765519499"


String iothubName = "cs147-hub-10";   
String deviceName = "cs147_project";  
String url = "https://" + iothubName +
             ".azure-devices.net/devices/" +
             deviceName +
             "/messages/events?api-version=2021-04-12";

// Telemetry interval
#define TELEMETRY_INTERVAL 3000  // Send data every 3 seconds

DHT20 dht;
uint32_t lastTelemetryTime = 0;

// =======================
// Utility: popcount for CAP1188
// =======================

static inline uint8_t popcount8(uint8_t x)
{
  // count set bits
  x = x - ((x >> 1) & 0x55);
  x = (x & 0x33) + ((x >> 2) & 0x33);
  return (((x + (x >> 4)) & 0x0F) * 0x01);
}

// =======================
// SETUP
// =======================

void setup()
{
  Serial.begin(9600);
  delay(1000);

  // LED + PWM setup
  pinMode(LED_PIN, OUTPUT);
  ledcSetup(LEDC_CH, LEDC_FREQ, LEDC_RES);
  ledcAttachPin(LED_PIN, LEDC_CH);
  ledcWrite(LEDC_CH, 0);

  Serial.println("Initializing I2C...");
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("CAP1188 test!");
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1) {
      delay(100);
    }
  }
  Serial.println("CAP1188 found!");

  // Photoregister initial reading
  int pin = analogRead(LDR_PIN);
  float volts = VREF * (float)pin / MAXADC;
  Serial.print("Initial LDR pin = ");
  Serial.println(pin);
  Serial.print("Initial LDR volts = ");
  Serial.println(volts);

  // LDR calibration
  Serial.println("Calibration started: expose to bright and dark areas");

  unsigned long t0 = millis();
  while (millis() - t0 < 10000) {
    int v = analogRead(LDR_PIN);

    if (v < minLight) minLight = v;
    if (v > maxLight) maxLight = v;

    // Blink LED during calibration
    digitalWrite(LED_PIN, (millis() / 300) % 2);
    delay(100);
  }

  // Turn off LED after calibration
  digitalWrite(LED_PIN, LOW);

  Serial.println("Calibration complete.");
  Serial.print("Min: ");
  Serial.println(minLight);
  Serial.print("Max: ");
  Serial.println(maxLight);

  int dhtStatus = dht.begin();
  if (dhtStatus != 0) {
    Serial.print("DHT20 init failed, status = ");
    Serial.println(dhtStatus);
  } else {
    Serial.println("DHT20 initialized successfully");
  }

  // Wi-Fi setup
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.println();
  Serial.println("Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
  }

  lastEnergyUpdate = millis();

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
}

// =======================
// LOOP
// =======================

void loop()
{
  uint32_t now = millis(); 
  // -----------------------------
  // 1) Touch sensor
  // -----------------------------
  uint8_t touched = cap.touched();
  uint8_t count   = popcount8(touched);
  bool isTouched  = (count > 0);

  if (isTouched && !wasTouched) {
    // Toggle LED enabled state on new touch
    ledEnabled = !ledEnabled;
    Serial.print("TOGGLE: LED is now ");
    Serial.println(ledEnabled ? "ENABLED" : "DISABLED");
  }
  wasTouched = isTouched;

  lastTouchCount = count;

  // -----------------------------
  // 2) LDR reading + LED brightness
  // -----------------------------
  int v = analogRead(LDR_PIN);
  lastLdrValue = v;

  // Map light to brightness: darker -> brighter
  int duty = map(v, minLight, maxLight, 4095, 0);
  duty = constrain(duty, 0, 4095);
  if (duty > 0 && duty < MIN_DUTY) {
    duty = MIN_DUTY;
  }

  if (!ledEnabled) {
    duty = 0;
  }

  ledcWrite(LEDC_CH, duty);

  // -----------------------------
  // 2.5) Energy savings accounting
  // -----------------------------
  float dt = (now - lastEnergyUpdate) / 1000.0f;  // seconds since last update
  if (dt < 0) dt = 0; 
  lastEnergyUpdate = now;

  if (ledEnabled) {
    // Baseline
    float baseline = dt;

    // Actual light brightness
    float actual   = dt * ((float)duty / 4095.0f);

    energyBaseline_s += baseline;
    energyActual_s   += actual;
  }

  Serial.print("touchCount="); Serial.print(count);
  Serial.print("  LDR="); Serial.print(v);
  Serial.print("  duty="); Serial.print(duty);
  Serial.print("  ledEnabled="); Serial.println(ledEnabled);

  // -----------------------------
  // 3) Telemetry to Azure (every TELEMETRY_INTERVAL ms)
  // -----------------------------
  if (now - lastTelemetryTime >= TELEMETRY_INTERVAL) {

    int status = dht.read();
    if (status != 0) {
      Serial.print("DHT20 read error, status = ");
      Serial.println(status);
      lastTelemetryTime = now;
      return;
    }

    float temperature = dht.getTemperature();
    float humidity    = dht.getHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read valid data from DHT20!");
      lastTelemetryTime = now;
      return;
    }

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Build JSON payload
  ArduinoJson::JsonDocument doc;
  doc["temperature"] = temperature;
  doc["humidity"]    = humidity;
  doc["ldr"]         = lastLdrValue;
  doc["ledEnabled"]  = ledEnabled;

  // Energy metrics
  float energySaved_s = energyBaseline_s - energyActual_s;
  doc["energy_baseline_s"] = energyBaseline_s;   // full brightness
  doc["energy_actual_s"]   = energyActual_s;     // actual brightness
  doc["energy_saved_s"]    = energySaved_s;      // baseline - actual
  if (energyBaseline_s > 0.0f) {
    doc["energy_saved_pct"] = 100.0f * energySaved_s / energyBaseline_s;
  }


    char buffer[256];
    serializeJson(doc, buffer, sizeof(buffer));

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", SAS_TOKEN);

    int httpCode = http.POST(buffer);

    Serial.print("HTTP code: ");
    Serial.println(httpCode);

    if (httpCode == 204) {
      Serial.print("Telemetry sent: ");
      Serial.println(buffer);
    } else {
      Serial.print("Failed to send telemetry. HTTP code: ");
      Serial.println(httpCode);
    }
    http.end();

    lastTelemetryTime = now;
  }

  delay(100);
}