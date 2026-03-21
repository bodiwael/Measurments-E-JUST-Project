/*
 * I2C Scanner
 * 
 * This sketch scans the I2C bus to find connected devices
 * and displays their addresses.
 * 
 * Use this to find your LCD I2C address (usually 0x27 or 0x3F)
 * 
 * Wiring:
 * SDA → GPIO 21
 * SCL → GPIO 22
 */

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nI2C Scanner");
  Serial.println("===========\n");
  
  Wire.begin(21, 22);  // SDA, SCL pins
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning I2C bus...\n");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("✓ Device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4) {
      Serial.print("✗ Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
    Serial.println("Check your wiring:");
    Serial.println("  - SDA to GPIO 21");
    Serial.println("  - SCL to GPIO 22");
    Serial.println("  - VCC to 5V");
    Serial.println("  - GND to GND\n");
  }
  else {
    Serial.print("\nTotal devices found: ");
    Serial.println(nDevices);
    Serial.println("\nCommon I2C Addresses:");
    Serial.println("  0x27 - LCD Display (common)");
    Serial.println("  0x3F - LCD Display (alternative)");
    Serial.println("  0x57 - EEPROM");
    Serial.println("  0x68 - RTC DS3231");
    Serial.println("  0x76 - BME280 Sensor\n");
  }

  delay(5000);  // Wait 5 seconds before next scan
}
