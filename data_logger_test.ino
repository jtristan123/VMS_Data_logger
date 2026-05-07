// =====================================================
// Teensy 4.1 + IR Temperature Sensor Test Layout
// =====================================================
//NOTE
// Sensor wiring:
//   Red   -> VIN (5V from USB-powered Teensy)
//   Black -> GND
//   White -> A0   (ONLY if signal is safe <= 3.3V)
//            If sensor output can go above 3.3V,
//            use a voltage divider before A0.
//
// This code is organized as a template so you can
// add features later without rewriting everything.
// =====================================================


// -------------------------
// Pin Definitions
// -------------------------
const int SENSOR_PIN = A0;      // Analog input pin for sensor signal
const int LED_PIN = 13;         // Built-in LED for status/debug


// -------------------------
// ADC / Voltage Settings
// -------------------------
const float ADC_REF_VOLTAGE = 3.3;   // Teensy analog reference voltage
const int ADC_MAX = 4095;            // 12-bit ADC max value


// -------------------------
// Sensor Conversion Settings
// -------------------------
//
// Adjust these if your sensor datasheet says different values.
//
// Example assumption from earlier discussion:
//   0.4V = 0°C
//   30mV per °C
//
const float SENSOR_OFFSET_V = 0.4;
const float SENSOR_SCALE_V_PER_C = 0.03;


// -------------------------
// Optional Divider Settings
// -------------------------
//
// If you are using a voltage divider on the sensor signal,
// set this to the factor needed to recover the real sensor voltage.
//
// Example:
//   If divider makes sensor voltage become 2/3 at pin,
//   then real sensor voltage = pin voltage * 1.5
//
// If NO divider is used, leave it as 1.0
//
const float DIVIDER_CORRECTION = 1.0;   // change to 1.5 if using 10k/20k divider


// -------------------------
// Averaging Settings
// -------------------------
const int NUM_SAMPLES = 10;     // number of samples to average


// -------------------------
// Timing Settings
// -------------------------
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL_MS = 500;


// =====================================================
// Setup
// =====================================================
void setup() {
  // Start serial monitor
  Serial.begin(115200);

  // Give time for serial monitor to open
  delay(1000);

  // Set ADC resolution
  analogReadResolution(12);

  // Built-in LED for debug/status
  pinMode(LED_PIN, OUTPUT);

  Serial.println("====================================");
  Serial.println("Teensy 4.1 IR Temperature Sensor Test");
  Serial.println("====================================");
  Serial.println("Starting...");
}


// =====================================================
// Main Loop
// =====================================================
void loop() {
  // Read at a fixed interval
  if (millis() - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = millis();

    // 1. Get averaged raw reading
    int rawADC = readAverageADC(SENSOR_PIN, NUM_SAMPLES);

    // 2. Convert ADC to pin voltage
    float pinVoltage = adcToVoltage(rawADC);

    // 3. Recover actual sensor voltage
    float sensorVoltage = pinVoltage * DIVIDER_CORRECTION; //i guess this is where the divider correction would go if needed

    // 4. Convert voltage to temperature
    float tempC = voltageToCelsius(sensorVoltage);
    float tempF = celsiusToFahrenheit(tempC);

    // 5. Print everything for debugging
    printReading(rawADC, pinVoltage, sensorVoltage, tempC, tempF);

    // 6. Optional safety/debug checks
    checkVoltageSafety(pinVoltage);

    // 7. Blink LED to show loop is alive
    blinkStatusLED();
  }

  // Future feature area:
  // - send to SD card
  // - trigger warning LED/buzzer
  // - send data over serial to another device
  // - send data over Wi-Fi/Bluetooth from another controller
}


// =====================================================
// Function: Read averaged ADC
// =====================================================
//
// Reads the analog pin multiple times and averages it
// to reduce noise.
//
// Returns:
//   averaged ADC value
//
int readAverageADC(int pin, int samples) {
  long total = 0;

  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delay(5);  // small pause between samples
  }

  return total / samples;
}


// =====================================================
// Function: Convert ADC reading to voltage
// =====================================================
//
// Formula:
//   voltage = (raw / ADC_MAX) * ADC_REF_VOLTAGE
//
float adcToVoltage(int rawADC) {
  return (rawADC / (float)ADC_MAX) * ADC_REF_VOLTAGE;
}


// =====================================================
// Function: Convert voltage to Celsius
// =====================================================
//
// Example formula:
//   tempC = (sensorVoltage - offset) / scale
//
float voltageToCelsius(float sensorVoltage) {
  return (sensorVoltage - SENSOR_OFFSET_V) / SENSOR_SCALE_V_PER_C;
}


// =====================================================
// Function: Convert Celsius to Fahrenheit
// =====================================================
float celsiusToFahrenheit(float tempC) {
  return (tempC * 9.0 / 5.0) + 32.0;
}


// =====================================================
// Function: Print reading
// =====================================================
//
// Prints raw ADC, voltage seen by Teensy pin,
// corrected sensor voltage, and estimated temperature.
//
void printReading(int rawADC, float pinVoltage, float sensorVoltage, float tempC, float tempF) {
  Serial.print("Raw ADC: ");
  Serial.print(rawADC);

  Serial.print(" | Pin V: ");
  Serial.print(pinVoltage, 3);

  Serial.print(" V | Sensor V: ");
  Serial.print(sensorVoltage, 3);

  Serial.print(" V | Temp C: ");
  Serial.print(tempC, 2);

  Serial.print(" | Temp F: ");
  Serial.println(tempF, 2);
}


// =====================================================
// Function: Check voltage safety
// =====================================================
//
// Warns if pin voltage is near or above safe Teensy range.
//
void checkVoltageSafety(float pinVoltage) {
  if (pinVoltage > 3.3) {
    Serial.println("WARNING: Pin voltage is ABOVE 3.3V! Unsafe for Teensy.");
  }
  else if (pinVoltage > 3.1) {
    Serial.println("CAUTION: Pin voltage is getting close to 3.3V.");
  }
}


// =====================================================
// Function: Blink status LED
// =====================================================
//
// Quick blink to show code is running.
//
void blinkStatusLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(20);
  digitalWrite(LED_PIN, LOW);
}


// =====================================================
// Future Feature Ideas maybe
// =====================================================
//
// 1. Add moving average filter
//    - smoother readings over time
//
// 2. Add min/max temperature tracking
//    - useful for tire temp or surface temp monitoring
//
// 3. Add threshold alert
//    - if temp too high, light LED or sound buzzer
//
// 4. Add SD card logging
//    - save timestamp + temperature
//
// 5. Add calibration values
//    - adjust offset and slope after testing
//
// 6. Add "sensor disconnected" detection
//    - if ADC is zero or floating wildly
//
// 7. Add serial command menu
//    - type commands in Serial Monitor to change settings
//
// 8. Add multiple sensors
//    - A0, A1, A2, etc.
//
