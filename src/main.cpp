// #include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_CAP1188.h>

// #define CAP1188_RESET 9
// #define CAP1188_CS 10

// //ESP32 I2C pin selection
// #define SDA_PIN 21
// #define SCL_PIN 22

// // Use I2C, no reset pin!
// Adafruit_CAP1188 cap = Adafruit_CAP1188();

// static inline uint8_t popcount8(uint8_t x)
// {
//   // count set bits
//   x = x - ((x >> 1) & 0x55);
//   x = (x & 0x33) + ((x >> 2) & 0x33);
//   return (((x + (x >> 4)) & 0x0F) * 0x01);
// }

// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("CAP1188 test!");
//   //initialize IsC on the chosen pins. On ESP32, SDA and SCL must be passed explicitly unless using default pins.
//   Wire.begin(SDA_PIN, SCL_PIN);

//   if (!cap.begin())
//   {
//     Serial.println("CAP1188 not found");
//     while (1) //Stop if sensor is not detected.
//       ;
//   }
//   Serial.println("CAP1188 found!");
// }

// void loop()
// {
//   //read touch status from CAP1188
//   uint8_t touched = cap.touched();
//   //Count how many pads are currently touched
//   uint8_t count = popcount8(touched);

//   //Force count to 0 is no pads are touched
//   if (touched == 0)
//   {
//     count = 0;
//   }
//   //Print number of touched pads
//   Serial.printf("count=%u\n", count);
//   delay(500);
// }

////////////////

// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_CAP1188.h>

// // --------- CAP1188 (I2C) ---------

// // ESP32 I2C pins
// #define SDA_PIN 21
// #define SCL_PIN 22

// // Use I2C, no reset pin (RESET tied to 3.3V on the board)
// Adafruit_CAP1188 cap = Adafruit_CAP1188();

// // Count set bits in a byte
// static inline uint8_t popcount8(uint8_t x) {
//   x = x - ((x >> 1) & 0x55);
//   x = (x & 0x33) + ((x >> 2) & 0x33);
//   return (((x + (x >> 4)) & 0x0F) * 0x01);
// }

// // --------- LDR / LED ---------

// const int LED_PIN = 33;
// const int LDR_PIN = 25;

// static const float VREF   = 3.3f;
// static const int   MAXADC = 4095;

// int minLight = 4095;
// int maxLight = 0;

// // --------- SETUP ---------

// void setup() {
//   Serial.begin(9600);
//   delay(1000);
//   Serial.println("\nCombined CAP1188 + LDR sketch");

//   // LED / LDR setup
//   pinMode(LED_PIN, OUTPUT);

//   // I2C + CAP1188
//   Wire.begin(SDA_PIN, SCL_PIN);

//   Serial.println("Initializing CAP1188 over I2C...");
//   bool ok = cap.begin();   // if needed later, you can try cap.begin(0x28) or (0x29)
//   Serial.print("cap.begin() returned: ");
//   Serial.println(ok ? "true" : "false");

//   if (!ok) {
//     Serial.println("CAP1188 not found – check I2C wiring and AD/RESET pins.");
//     while (1) {
//       delay(1000);  // keep watchdog happy while halting
//     }
//   }
//   Serial.println("CAP1188 found!");

//   // LDR calibration
//   int pin = analogRead(LDR_PIN);
//   float volts = VREF * (float)pin / MAXADC;
//   Serial.print("Initial LDR reading: pin=");
//   Serial.println(pin);
//   Serial.print("Initial voltage=");
//   Serial.println(volts);

//   Serial.println("Calibration started: expose sensor to bright and dark areas for 10 seconds...");

//   unsigned long t0 = millis();
//   while (millis() - t0 < 10000) {  // 10 seconds
//     int v = analogRead(LDR_PIN);

//     if (v < minLight) minLight = v;
//     if (v > maxLight) maxLight = v;

//     // Blink LED during calibration (toggle every 300ms)
//     digitalWrite(LED_PIN, (millis() / 300) % 2);
//     delay(100);
//   }

//   // Turn off LED after calibration
//   digitalWrite(LED_PIN, LOW);

//   Serial.println("Calibration complete.");
//   Serial.print("Min light: "); Serial.println(minLight);
//   Serial.print("Max light: "); Serial.println(maxLight);
// }

// // --------- LOOP ---------

// void loop() {
//   // LDR reading
//   int raw   = analogRead(LDR_PIN);
//   float volts = VREF * (float)raw / MAXADC;

//   // CAP1188 touch reading
//   uint8_t touched    = cap.touched();
//   uint8_t touchCount = popcount8(touched);
//   if (touched == 0) {
//     touchCount = 0;
//   }

//   // Print status
//   Serial.print("LDR pin="); Serial.print(raw);
//   Serial.print("  V=");     Serial.print(volts, 3);

//   Serial.print("  |  CAP1188 touched=0x");
//   Serial.print(touched, HEX);
//   Serial.print("  (count=");
//   Serial.print(touchCount);
//   Serial.println(")");

//   delay(200);
// }

//////////////////

// #include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_CAP1188.h>

// const int LED_PIN = 33;
// const int LDR_PIN = 25;

// static const float VREF = 3.3f;
// static const int MAXADC = 4095;

// const int LEDC_CH   = 0;
// const int LEDC_FREQ = 5000;
// const int LEDC_RES  = 8;
// const int MIN_DUTY  = 60;

// #define CAP1188_RESET 9
// #define CAP1188_CS 10

// //ESP32 I2C pin selection
// #define SDA_PIN 21
// #define SCL_PIN 22

// // Use I2C, no reset pin!
// Adafruit_CAP1188 cap = Adafruit_CAP1188();

// static inline uint8_t popcount8(uint8_t x)
// {
//   // count set bits
//   x = x - ((x >> 1) & 0x55);
//   x = (x & 0x33) + ((x >> 2) & 0x33);
//   return (((x + (x >> 4)) & 0x0F) * 0x01);
// }

// void setup()
// {
//   Serial.begin(9600);

//   pinMode(LED_PIN, OUTPUT);

//   digitalWrite(LED_PIN, LOW);

//   // ledcSetup(LEDC_CH, LEDC_FREQ, LEDC_RES);
//   // ledcAttachPin(LED_PIN, LEDC_CH);

//   // ledcWrite(LEDC_CH, 0)

//   Serial.println("CAP1188 test!");
//   //initialize IsC on the chosen pins. On ESP32, SDA and SCL must be passed explicitly unless using default pins.
//   Wire.begin(SDA_PIN, SCL_PIN);

//   if (!cap.begin())
//   {
//     Serial.println("CAP1188 not found");
//     while (1) //Stop if sensor is not detected.
//       ;
//   }
//   Serial.println("CAP1188 found!");

//   //photoregister
//   int pin = analogRead(25); //read analog value
//   float volts = VREF * (float)pin / MAXADC; //convert integer value to voltage value
//   Serial.print("pin=");
//   Serial.println(pin);
//   Serial.print("\nvolt=");
//   Serial.println(volts);
//   delay(200);
// }

// void loop()
// {
//   //read touch status from CAP1188
//   uint8_t touched = cap.touched();
//   //Count how many pads are currently touched
//   uint8_t count = popcount8(touched);

//   //Force count to 0 is no pads are touched
//   if (touched == 0)
//   {
//     count = 0;
//   }
//   //Print number of touched pads
//   Serial.printf("count=%u\n", count);

//   int state = digitalRead(LED_PIN);

//   if(count > 0 && state == HIGH) {
//     digitalWrite(LED_PIN, LOW);
//   } else if(count > 0 && state == LOW) {
//     digitalWrite(LED_PIN, HIGH);
//   }
//   delay(500);
// }

/////////////////

////////////////////////////////// below is the first version

// #include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_CAP1188.h>

// bool ledEnabled = true;
// bool wasTouched = false; 

// const int LED_PIN = 33;
// const int LDR_PIN = 25;

// static const float VREF = 3.3f;
// static const int MAXADC = 4095;

// const int LEDC_CH = 0;
// const int LEDC_FREQ = 5000;
// const int LEDC_RES = 12;
// const int MIN_DUTY = 60;

// #define CAP1188_RESET 9
// #define CAP1188_CS 10

// // ESP32 I2C pin selection
// #define SDA_PIN 21
// #define SCL_PIN 22

// int minLight = 4095;
// int maxLight = 0;

// // Use I2C, no reset pin!
// Adafruit_CAP1188 cap = Adafruit_CAP1188();

// static inline uint8_t popcount8(uint8_t x)
// {
//   // count set bits
//   x = x - ((x >> 1) & 0x55);
//   x = (x & 0x33) + ((x >> 2) & 0x33);
//   return (((x + (x >> 4)) & 0x0F) * 0x01);
// }

// void setup()
// {
//   Serial.begin(9600);

//   pinMode(LED_PIN, OUTPUT);

//   // digitalWrite(LED_PIN, LOW);

//   ledcSetup(LEDC_CH, LEDC_FREQ, LEDC_RES);
//   ledcAttachPin(LED_PIN, LEDC_CH);

//   ledcWrite(LEDC_CH, 0);

//   Serial.println("CAP1188 test!");
//   // initialize IsC on the chosen pins. On ESP32, SDA and SCL must be passed explicitly unless using default pins.
//   Wire.begin(SDA_PIN, SCL_PIN);

//   if (!cap.begin())
//   {
//     Serial.println("CAP1188 not found");
//     while (1) // Stop if sensor is not detected.
//       ;
//   }
//   Serial.println("CAP1188 found!");

//   // photoregister
//   int pin = analogRead(25);                 // read analog value
//   float volts = VREF * (float)pin / MAXADC; // convert integer value to voltage value
//   Serial.print("pin=");
//   Serial.println(pin);
//   Serial.print("\nvolt=");
//   Serial.println(volts);
//   delay(200);

//   Serial.println("Calibration started: expose to bright and dark areas");

//   unsigned long t0 = millis();
//   while (millis() - t0 < 10000)
//   { // run the loop for 10 seconds
//     // read the current light level from the photoresister divider
//     int v = analogRead(LDR_PIN);
//     // update minimum and maximum available light value
//     if (v < minLight)
//       minLight = v;
//     if (v > maxLight)
//       maxLight = v;

//     // Blink LED during calibration, toggles every 300ms
//     digitalWrite(LED_PIN, (millis() / 300) % 2);
//     delay(100); // sleep 100mn between samples
//   }
//   // turn of the LED after calibration stage
//   digitalWrite(LED_PIN, LOW);

//   Serial.println("Calibration complete.");
//   Serial.println("Min: ");
//   Serial.println(minLight);
//   Serial.println("Max: ");
//   Serial.println(maxLight);
// }

// void loop()
// {
//   // --- 1) Read touch from CAP1188 ---
//   uint8_t touched = cap.touched();
//   uint8_t count   = popcount8(touched);
//   bool isTouched  = (count > 0);

//   // --- 2) Toggle LED enabled state on NEW touch (rising edge) ---
//   if (isTouched && !wasTouched) {
//     ledEnabled = !ledEnabled;
//     Serial.print("TOGGLE: LED is now ");
//     Serial.println(ledEnabled ? "ENABLED" : "DISABLED");
//   }
//   wasTouched = isTouched;

//   // --- 3) Read light sensor ---
//   int v = analogRead(LDR_PIN);

//   // Map light to brightness: darker -> brighter
//   int duty = map(v, minLight, maxLight, 4095, 0);
//   duty = constrain(duty, 0, 4095);
//   if (duty > 0 && duty < MIN_DUTY) {
//     duty = MIN_DUTY;
//   }

//   // If LED is disabled by touch, force off
//   if (!ledEnabled) {
//     duty = 0;
//   }

//   // --- 4) Apply PWM once ---
//   ledcWrite(LEDC_CH, duty);

//   // Debug prints
//   Serial.print("count="); Serial.print(count);
//   Serial.print("  LDR="); Serial.print(v);
//   Serial.print("  duty="); Serial.print(duty);
//   Serial.print("  ledEnabled="); Serial.println(ledEnabled);

//   delay(100);
// }

////////////////////////////////// ending1

// #include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_CAP1188.h>

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <HTTPClient.h>
// #include "DHT20.h"
// #include <ArduinoJson.h>
// #include <string>

// // =======================
// // Hardware config (LED, LDR, CAP1188)
// // =======================

// bool ledEnabled = true;
// bool wasTouched = false;

// const int LED_PIN = 33;
// const int LDR_PIN = 35;

// static const float VREF = 3.3f;
// static const int MAXADC = 4095;

// const int LEDC_CH   = 0;
// const int LEDC_FREQ = 5000;
// const int LEDC_RES  = 12;
// const int MIN_DUTY  = 60;

// #define CAP1188_RESET 9   // not used (I2C mode)
// #define CAP1188_CS    10  // not used (I2C mode)

// // ESP32 I2C pin selection (shared by CAP1188 + DHT20)
// #define SDA_PIN 21
// #define SCL_PIN 22

// int minLight = 4095;
// int maxLight = 0;

// // Use I2C, no reset pin!
// Adafruit_CAP1188 cap = Adafruit_CAP1188();

// // For remembering latest readings (for telemetry)
// int     lastLdrValue      = 0;
// uint8_t lastTouchCount    = 0;

// // =======================
// // Wi-Fi + Azure IoT config
// // =======================

// // ⚠️ For real use, avoid UCInet Mobile Access (captive portal).
// // Use a hotspot or normal WPA2 Wi-Fi if possible.
// #define WIFI_SSID     "UCInet Mobile Access"
// #define WIFI_PASSWORD ""

// // Azure IoT Hub configuration
// #define SAS_TOKEN "SharedAccessSignature sr=cs147-hub-10.azure-devices.net%2Fdevices%2Fcs147_project&sig=IAHBE00v8AN7jaGhlCQeKPY5S%2BD7FVzlfXJ79FwhR2A%3D&se=1765499158"

// // (You can keep root_ca if you want proper TLS later, but we'll ignore it for now)
// // const char* root_ca = "-----BEGIN CERTIFICATE-----\n...";

// String iothubName = "cs147-hub-10";      // Your hub name
// String deviceName = "cs147_project";     // Your device name
// String url = "https://" + iothubName +
//              ".azure-devices.net/devices/" +
//              deviceName +
//              "/messages/events?api-version=2021-04-12";

// // Telemetry interval
// #define TELEMETRY_INTERVAL 3000  // Send data every 3 seconds

// DHT20 dht;
// uint32_t lastTelemetryTime = 0;

// // =======================
// // Utility: popcount for CAP1188
// // =======================

// static inline uint8_t popcount8(uint8_t x)
// {
//   // count set bits
//   x = x - ((x >> 1) & 0x55);
//   x = (x & 0x33) + ((x >> 2) & 0x33);
//   return (((x + (x >> 4)) & 0x0F) * 0x01);
// }

// // =======================
// // SETUP
// // =======================

// void setup()
// {
//   Serial.begin(9600);
//   delay(1000);

//   // LED + PWM setup
//   pinMode(LED_PIN, OUTPUT);
//   ledcSetup(LEDC_CH, LEDC_FREQ, LEDC_RES);
//   ledcAttachPin(LED_PIN, LEDC_CH);
//   ledcWrite(LEDC_CH, 0);

//   // I2C for CAP1188 + DHT20
//   Serial.println("Initializing I2C...");
//   Wire.begin(SDA_PIN, SCL_PIN);

//   // CAP1188 init
//   Serial.println("CAP1188 test!");
//   if (!cap.begin()) {
//     Serial.println("CAP1188 not found");
//     while (1) {
//       delay(100);
//     }
//   }
//   Serial.println("CAP1188 found!");

//   // Photoregister initial reading
//   int pin = analogRead(LDR_PIN);
//   float volts = VREF * (float)pin / MAXADC;
//   Serial.print("Initial LDR pin = ");
//   Serial.println(pin);
//   Serial.print("Initial LDR volts = ");
//   Serial.println(volts);

//   // LDR calibration
//   Serial.println("Calibration started: expose to bright and dark areas");

//   unsigned long t0 = millis();
//   while (millis() - t0 < 10000) {  // 10 seconds
//     int v = analogRead(LDR_PIN);

//     if (v < minLight) minLight = v;
//     if (v > maxLight) maxLight = v;

//     // Blink LED during calibration
//     digitalWrite(LED_PIN, (millis() / 300) % 2);
//     delay(100);
//   }

//   // Turn off LED after calibration
//   digitalWrite(LED_PIN, LOW);

//   Serial.println("Calibration complete.");
//   Serial.print("Min: ");
//   Serial.println(minLight);
//   Serial.print("Max: ");
//   Serial.println(maxLight);

//   // DHT20 init
//   int dhtStatus = dht.begin();
//   if (dhtStatus != 0) {
//     Serial.print("DHT20 init failed, status = ");
//     Serial.println(dhtStatus);
//   } else {
//     Serial.println("DHT20 initialized successfully");
//   }

//   // Wi-Fi setup
//   WiFi.mode(WIFI_STA);
//   delay(1000);
//   Serial.println();
//   Serial.println("Connecting to Wi-Fi: ");
//   Serial.println(WIFI_SSID);

//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//     Serial.print(WiFi.status());
//   }

//   Serial.println();
//   Serial.println("WiFi connected");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
//   Serial.print("MAC address: ");
//   Serial.println(WiFi.macAddress());
// }

// // =======================
// // LOOP
// // =======================

// void loop()
// {
//   // -----------------------------
//   // 1) Touch sensor (CAP1188)
//   // -----------------------------
//   uint8_t touched = cap.touched();
//   uint8_t count   = popcount8(touched);
//   bool isTouched  = (count > 0);

//   if (isTouched && !wasTouched) {
//     // Toggle LED enabled state on new touch
//     ledEnabled = !ledEnabled;
//     Serial.print("TOGGLE: LED is now ");
//     Serial.println(ledEnabled ? "ENABLED" : "DISABLED");
//   }
//   wasTouched = isTouched;

//   lastTouchCount = count;

//   // -----------------------------
//   // 2) LDR reading + LED brightness
//   // -----------------------------
//   int v = analogRead(LDR_PIN);
//   lastLdrValue = v;

//   // Map light to brightness: darker -> brighter
//   int duty = map(v, minLight, maxLight, 4095, 0);
//   duty = constrain(duty, 0, 4095);
//   if (duty > 0 && duty < MIN_DUTY) {
//     duty = MIN_DUTY;
//   }

//   if (!ledEnabled) {
//     duty = 0;
//   }

//   ledcWrite(LEDC_CH, duty);

//   // Debug prints for local behavior
//   Serial.print("touchCount="); Serial.print(count);
//   Serial.print("  LDR="); Serial.print(v);
//   Serial.print("  duty="); Serial.print(duty);
//   Serial.print("  ledEnabled="); Serial.println(ledEnabled);

//   // -----------------------------
//   // 3) Telemetry to Azure (every TELEMETRY_INTERVAL ms)
//   // -----------------------------
//   uint32_t now = millis();
//   if (now - lastTelemetryTime >= TELEMETRY_INTERVAL) {

//     // Read DHT20 before sending
//     int status = dht.read();
//     if (status != 0) {
//       Serial.print("DHT20 read error, status = ");
//       Serial.println(status);
//       // For debugging you can still send dummy values if you want
//       // but here we'll skip send
//       lastTelemetryTime = now;
//       return;
//     }

//     float temperature = dht.getTemperature();
//     float humidity    = dht.getHumidity();

//     if (isnan(temperature) || isnan(humidity)) {
//       Serial.println("Failed to read valid data from DHT20!");
//       lastTelemetryTime = now;
//       return;
//     }

//     Serial.print("Temp: ");
//     Serial.print(temperature);
//     Serial.print(" °C, Humidity: ");
//     Serial.print(humidity);
//     Serial.println(" %");

//     // Build JSON payload
//     ArduinoJson::JsonDocument doc;
//     doc["temperature"] = temperature;
//     doc["humidity"]    = humidity;
//     doc["ldr"]         = lastLdrValue;
//     doc["touchCount"]  = lastTouchCount;
//     doc["ledEnabled"]  = ledEnabled;

//     char buffer[256];
//     serializeJson(doc, buffer, sizeof(buffer));

//     // Send telemetry via HTTPS
//     WiFiClientSecure client;
//     // For debugging: ignore certificate validation.
//     // Later you can replace this with setCACert(root_ca) and proper time.
//     client.setInsecure();

//     HTTPClient http;
//     http.begin(client, url);
//     http.addHeader("Content-Type", "application/json");
//     http.addHeader("Authorization", SAS_TOKEN);

//     int httpCode = http.POST(buffer);

//     Serial.print("HTTP code: ");
//     Serial.println(httpCode);

//     if (httpCode == 204) {
//       Serial.print("Telemetry sent: ");
//       Serial.println(buffer);
//     } else {
//       Serial.print("Failed to send telemetry. HTTP code: ");
//       Serial.println(httpCode);
//     }
//     http.end();

//     lastTelemetryTime = now;
//   }

//   delay(100);  // keep main loop responsive but not too fast
// }



////////////////////// ending for first

// #include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_CAP1188.h>

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <HTTPClient.h>
// #include "DHT20.h"
// #include <ArduinoJson.h>
// #include <string>

// // =======================
// // Hardware config (LED, LDR, CAP1188)
// // =======================

// bool ledEnabled = true;
// bool wasTouched = false;

// const int LED_PIN = 33;
// const int LDR_PIN = 35;

// static const float VREF = 3.3f;
// static const int MAXADC = 4095;

// const int LEDC_CH   = 0;
// const int LEDC_FREQ = 5000;
// const int LEDC_RES  = 12;
// const int MIN_DUTY  = 60;

// #define CAP1188_RESET 9   // not used (I2C mode)
// #define CAP1188_CS    10  // not used (I2C mode)

// // ESP32 I2C pin selection (shared by CAP1188 + DHT20)
// #define SDA_PIN 21
// #define SCL_PIN 22
// int minLight = 4095;
// int maxLight = 0;

// // Use I2C, no reset pin!
// Adafruit_CAP1188 cap = Adafruit_CAP1188();

// // For remembering latest readings (for telemetry)
// int     lastLdrValue      = 0;
// uint8_t lastTouchCount    = 0;
// float energyBaseline_s = 0.0f;   // "full-brightness seconds"
// float energyActual_s   = 0.0f;   // "actual-brightness seconds"
// uint32_t lastEnergyUpdate = 0;

// // =======================
// // Wi-Fi + Azure IoT config
// // =======================

// // ⚠️ For real use, avoid UCInet Mobile Access (captive portal).
// // Use a hotspot or normal WPA2 Wi-Fi if possible.
// #define WIFI_SSID     "UCInet Mobile Access"
// #define WIFI_PASSWORD ""

// // Azure IoT Hub configuration
// #define SAS_TOKEN "SharedAccessSignature sr=cs147-hub-10.azure-devices.net%2Fdevices%2Fcs147_project&sig=G%2F%2B%2BpIg8wrwhmcEUG1XKkdThrCVJZpoUItISKV1YrgU%3D&se=1765515474"

// // (You can keep root_ca if you want proper TLS later, but we'll ignore it for now)
// // const char* root_ca = "-----BEGIN CERTIFICATE-----\n...";

// String iothubName = "cs147-hub-10";      // Your hub name
// String deviceName = "cs147_project";     // Your device name
// String url = "https://" + iothubName +
//              ".azure-devices.net/devices/" +
//              deviceName +
//              "/messages/events?api-version=2021-04-12";

// // Telemetry interval
// #define TELEMETRY_INTERVAL 3000  // Send data every 3 seconds

// DHT20 dht;
// uint32_t lastTelemetryTime = 0;

// // =======================
// // Utility: popcount for CAP1188
// // =======================

// static inline uint8_t popcount8(uint8_t x)
// {
//   // count set bits
//   x = x - ((x >> 1) & 0x55);
//   x = (x & 0x33) + ((x >> 2) & 0x33);
//   return (((x + (x >> 4)) & 0x0F) * 0x01);
// }

// // =======================
// // SETUP
// // =======================

// void setup()
// {
//   Serial.begin(9600);
//   delay(1000);

//   // LED + PWM setup
//   pinMode(LED_PIN, OUTPUT);
//   ledcSetup(LEDC_CH, LEDC_FREQ, LEDC_RES);
//   ledcAttachPin(LED_PIN, LEDC_CH);
//   ledcWrite(LEDC_CH, 0);

//   // I2C for CAP1188 + DHT20
//   Serial.println("Initializing I2C...");
//   Wire.begin(SDA_PIN, SCL_PIN);

//   // CAP1188 init
//   Serial.println("CAP1188 test!");
//   if (!cap.begin()) {
//     Serial.println("CAP1188 not found");
//     while (1) {
//       delay(100);
//     }
//   }
//   Serial.println("CAP1188 found!");

//   // Photoregister initial reading
//   int pin = analogRead(LDR_PIN);
//   float volts = VREF * (float)pin / MAXADC;
//   Serial.print("Initial LDR pin = ");
//   Serial.println(pin);
//   Serial.print("Initial LDR volts = ");
//   Serial.println(volts);

//   // LDR calibration
//   Serial.println("Calibration started: expose to bright and dark areas");

//   unsigned long t0 = millis();
//   while (millis() - t0 < 10000) {  // 10 seconds
//     int v = analogRead(LDR_PIN);

//     if (v < minLight) minLight = v;
//     if (v > maxLight) maxLight = v;

//     // Blink LED during calibration
//     digitalWrite(LED_PIN, (millis() / 300) % 2);
//     delay(100);
//   }

//   // Turn off LED after calibration
//   digitalWrite(LED_PIN, LOW);

//   Serial.println("Calibration complete.");
//   Serial.print("Min: ");
//   Serial.println(minLight);
//   Serial.print("Max: ");
//   Serial.println(maxLight);

//   // DHT20 init
//   int dhtStatus = dht.begin();
//   if (dhtStatus != 0) {
//     Serial.print("DHT20 init failed, status = ");
//     Serial.println(dhtStatus);
//   } else {
//     Serial.println("DHT20 initialized successfully");
//   }

//   // Wi-Fi setup
//   WiFi.mode(WIFI_STA);
//   delay(1000);
//   Serial.println();
//   Serial.println("Connecting to Wi-Fi: ");
//   Serial.println(WIFI_SSID);

//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//     Serial.print(WiFi.status());
//   }

//   lastEnergyUpdate = millis();

//   Serial.println();
//   Serial.println("WiFi connected");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
//   Serial.print("MAC address: ");
//   Serial.println(WiFi.macAddress());
// }

// // =======================
// // LOOP
// // =======================

// void loop()
// {
//   uint32_t now = millis(); 
//   // -----------------------------
//   // 1) Touch sensor (CAP1188)
//   // -----------------------------
//   uint8_t touched = cap.touched();
//   uint8_t count   = popcount8(touched);
//   bool isTouched  = (count > 0);

//   if (isTouched && !wasTouched) {
//     // Toggle LED enabled state on new touch
//     ledEnabled = !ledEnabled;
//     Serial.print("TOGGLE: LED is now ");
//     Serial.println(ledEnabled ? "ENABLED" : "DISABLED");
//   }
//   wasTouched = isTouched;

//   lastTouchCount = count;

//   // -----------------------------
//   // 2) LDR reading + LED brightness
//   // -----------------------------
//   int v = analogRead(LDR_PIN);
//   lastLdrValue = v;

//   // Map light to brightness: darker -> brighter
//   int duty = map(v, minLight, maxLight, 4095, 0);
//   duty = constrain(duty, 0, 4095);
//   if (duty > 0 && duty < MIN_DUTY) {
//     duty = MIN_DUTY;
//   }

//   if (!ledEnabled) {
//     duty = 0;
//   }

//   ledcWrite(LEDC_CH, duty);

//   // -----------------------------
//   // 2.5) Energy savings accounting
//   // -----------------------------
//   float dt = (now - lastEnergyUpdate) / 1000.0f;  // seconds since last update
//   if (dt < 0) dt = 0;  // safety
//   lastEnergyUpdate = now;

//   if (ledEnabled) {
//     // Baseline: LED would have been full brightness during dt
//     float baseline = dt;

//     // Actual: scaled by duty (0–4095)
//     float actual   = dt * ((float)duty / 4095.0f);

//     energyBaseline_s += baseline;
//     energyActual_s   += actual;
//   }

//   // Debug prints for local behavior
//   Serial.print("touchCount="); Serial.print(count);
//   Serial.print("  LDR="); Serial.print(v);
//   Serial.print("  duty="); Serial.print(duty);
//   Serial.print("  ledEnabled="); Serial.println(ledEnabled);

//   // -----------------------------
//   // 3) Telemetry to Azure (every TELEMETRY_INTERVAL ms)
//   // -----------------------------
//   // uint32_t now = millis();
//   if (now - lastTelemetryTime >= TELEMETRY_INTERVAL) {

//     // Read DHT20 before sending
//     int status = dht.read();
//     if (status != 0) {
//       Serial.print("DHT20 read error, status = ");
//       Serial.println(status);
//       // For debugging you can still send dummy values if you want
//       // but here we'll skip send
//       lastTelemetryTime = now;
//       return;
//     }

//     float temperature = dht.getTemperature();
//     float humidity    = dht.getHumidity();

//     if (isnan(temperature) || isnan(humidity)) {
//       Serial.println("Failed to read valid data from DHT20!");
//       lastTelemetryTime = now;
//       return;
//     }

//     Serial.print("Temp: ");
//     Serial.print(temperature);
//     Serial.print(" °C, Humidity: ");
//     Serial.print(humidity);
//     Serial.println(" %");

//     // Build JSON payload
//   ArduinoJson::JsonDocument doc;
//   doc["temperature"] = temperature;
//   doc["humidity"]    = humidity;
//   doc["ldr"]         = lastLdrValue;
//   doc["touchCount"]  = lastTouchCount;
//   doc["ledEnabled"]  = ledEnabled;

//   // Energy metrics
//   float energySaved_s = energyBaseline_s - energyActual_s;
//   doc["energy_baseline_s"] = energyBaseline_s;   // seconds at full brightness
//   doc["energy_actual_s"]   = energyActual_s;     // seconds at actual brightness
//   doc["energy_saved_s"]    = energySaved_s;      // baseline - actual
//   if (energyBaseline_s > 0.0f) {
//     doc["energy_saved_pct"] = 100.0f * energySaved_s / energyBaseline_s;
//   }


//     char buffer[256];
//     serializeJson(doc, buffer, sizeof(buffer));

//     // Send telemetry via HTTPS
//     WiFiClientSecure client;
//     // For debugging: ignore certificate validation.
//     // Later you can replace this with setCACert(root_ca) and proper time.
//     client.setInsecure();

//     HTTPClient http;
//     http.begin(client, url);
//     http.addHeader("Content-Type", "application/json");
//     http.addHeader("Authorization", SAS_TOKEN);

//     int httpCode = http.POST(buffer);

//     Serial.print("HTTP code: ");
//     Serial.println(httpCode);

//     if (httpCode == 204) {
//       Serial.print("Telemetry sent: ");
//       Serial.println(buffer);
//     } else {
//       Serial.print("Failed to send telemetry. HTTP code: ");
//       Serial.println(httpCode);
//     }
//     http.end();

//     lastTelemetryTime = now;
//   }

//   delay(100);  // keep main loop responsive but not too fast
// }

/////////////////////

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

#define CAP1188_RESET 9   // not used (I2C mode)
#define CAP1188_CS    10  // not used (I2C mode)

// ESP32 I2C pin selection (shared by CAP1188 + DHT20)
#define SDA_PIN 21
#define SCL_PIN 22
int minLight = 4095;
int maxLight = 0;

// Use I2C, no reset pin!
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// For remembering latest readings (for telemetry)
int     lastLdrValue      = 0;
uint8_t lastTouchCount    = 0;
float energyBaseline_s = 0.0f;   // "full-brightness seconds"
float energyActual_s   = 0.0f;   // "actual-brightness seconds"
uint32_t lastEnergyUpdate = 0;

// =======================
// Wi-Fi + Azure IoT config
// =======================

// ⚠️ For real use, avoid UCInet Mobile Access (captive portal).
// Use a hotspot or normal WPA2 Wi-Fi if possible.
#define WIFI_SSID     "UCInet Mobile Access"
#define WIFI_PASSWORD ""

// Azure IoT Hub configuration
#define SAS_TOKEN "SharedAccessSignature sr=cs147-hub-10.azure-devices.net%2Fdevices%2Fcs147_project&sig=WXH2LPjvNS6ZBKqyHrobw0upIiRJyEvnwOKdWQ8bYsg%3D&se=1765519499"

// (You can keep root_ca if you want proper TLS later, but we'll ignore it for now)
// const char* root_ca = "-----BEGIN CERTIFICATE-----\n...";

String iothubName = "cs147-hub-10";      // Your hub name
String deviceName = "cs147_project";     // Your device name
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

  // I2C for CAP1188 + DHT20
  Serial.println("Initializing I2C...");
  Wire.begin(SDA_PIN, SCL_PIN);

  // CAP1188 init
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
  while (millis() - t0 < 10000) {  // 10 seconds
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

  // DHT20 init
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
  // 1) Touch sensor (CAP1188)
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
  if (dt < 0) dt = 0;  // safety
  lastEnergyUpdate = now;

  if (ledEnabled) {
    // Baseline: LED would have been full brightness during dt
    float baseline = dt;

    // Actual: scaled by duty (0–4095)
    float actual   = dt * ((float)duty / 4095.0f);

    energyBaseline_s += baseline;
    energyActual_s   += actual;
  }

  // Debug prints for local behavior
  Serial.print("touchCount="); Serial.print(count);
  Serial.print("  LDR="); Serial.print(v);
  Serial.print("  duty="); Serial.print(duty);
  Serial.print("  ledEnabled="); Serial.println(ledEnabled);

  // -----------------------------
  // 3) Telemetry to Azure (every TELEMETRY_INTERVAL ms)
  // -----------------------------
  // uint32_t now = millis();
  if (now - lastTelemetryTime >= TELEMETRY_INTERVAL) {

    // Read DHT20 before sending
    int status = dht.read();
    if (status != 0) {
      Serial.print("DHT20 read error, status = ");
      Serial.println(status);
      // For debugging you can still send dummy values if you want
      // but here we'll skip send
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
  // doc["touchCount"]  = lastTouchCount;
  doc["ledEnabled"]  = ledEnabled;

  // Energy metrics
  float energySaved_s = energyBaseline_s - energyActual_s;
  doc["energy_baseline_s"] = energyBaseline_s;   // seconds at full brightness
  doc["energy_actual_s"]   = energyActual_s;     // seconds at actual brightness
  doc["energy_saved_s"]    = energySaved_s;      // baseline - actual
  if (energyBaseline_s > 0.0f) {
    doc["energy_saved_pct"] = 100.0f * energySaved_s / energyBaseline_s;
  }


    char buffer[256];
    serializeJson(doc, buffer, sizeof(buffer));

    // Send telemetry via HTTPS
    WiFiClientSecure client;
    // For debugging: ignore certificate validation.
    // Later you can replace this with setCACert(root_ca) and proper time.
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

  delay(100);  // keep main loop responsive but not too fast
}