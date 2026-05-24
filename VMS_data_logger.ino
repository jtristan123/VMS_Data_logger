// Simple IR sensor test for Teensy 4.1

int sensorPin = A0;   // white wire goes here

void setup() {
  Serial.begin(115200);        // start serial monitor
  analogReadResolution(12);    // Teensy reads 0 to 4095
  delay(1000);                 // wait a moment
  Serial.println("Sensor test start");
}

void loop() {
  int rawValue = analogRead(sensorPin);   // read sensor
  float voltage = (rawValue / 4095.0) * 3.3;   // convert to voltage

  Serial.print("Raw: ");
  Serial.print(rawValue);
  Serial.print("   Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(500);   // wait half a second
}
