/*
 * Sensor Test Sketch
 * 
 * This sketch tests all sensors individually to verify connections
 * and readings before running the main program.
 * 
 * Upload this first to ensure all hardware is working correctly.
 */

// Pin Definitions
#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define LDR_PIN 32
#define PANEL_TEMP_PIN 33
#define AMBIENT_TEMP_PIN 25

// Calibration Constants
#define VOLTAGE_DIVIDER_RATIO 11.0
#define ACS712_SENSITIVITY 0.066
#define ACS712_OFFSET 2.5

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configure ADC
  analogReadResolution(12);  // 12-bit (0-4095)
  analogSetAttenuation(ADC_11db);  // 0-3.3V
  
  // Configure pins
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(CURRENT_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(PANEL_TEMP_PIN, INPUT);
  pinMode(AMBIENT_TEMP_PIN, INPUT);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   SENSOR TEST PROGRAM              ║");
  Serial.println("║   Solar Panel Monitoring System    ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  Serial.println("Testing all sensors...\n");
  delay(2000);
}

void loop() {
  Serial.println("═══════════════════════════════════════");
  Serial.println("        SENSOR READINGS");
  Serial.println("═══════════════════════════════════════\n");
  
  // Test Voltage Sensor
  testVoltageSensor();
  
  // Test Current Sensor
  testCurrentSensor();
  
  // Test Irradiance Sensor (LDR)
  testLDR();
  
  // Test Temperature Sensors
  testTemperatureSensors();
  
  Serial.println("\n═══════════════════════════════════════\n");
  Serial.println("Waiting 3 seconds before next reading...\n");
  delay(3000);
}

// ═══════════════════════════════════════════════════════════
// VOLTAGE SENSOR TEST
// ═══════════════════════════════════════════════════════════
void testVoltageSensor() {
  Serial.println("1. VOLTAGE SENSOR (Voltage Divider)");
  Serial.println("   Pin: GPIO34");
  Serial.println("   ─────────────────────────────────");
  
  int rawADC = analogRead(VOLTAGE_PIN);
  float adcVoltage = (rawADC / 4095.0) * 3.3;
  float panelVoltage = adcVoltage * VOLTAGE_DIVIDER_RATIO;
  
  Serial.print("   Raw ADC: ");
  Serial.print(rawADC);
  Serial.println(" (0-4095)");
  
  Serial.print("   ADC Voltage: ");
  Serial.print(adcVoltage, 3);
  Serial.println(" V");
  
  Serial.print("   Panel Voltage: ");
  Serial.print(panelVoltage, 2);
  Serial.println(" V");
  
  // Status check
  if (rawADC < 10) {
    Serial.println("   ⚠️  WARNING: Very low reading - check connections!");
  } else if (panelVoltage > 25) {
    Serial.println("   ⚠️  WARNING: Voltage too high - check divider!");
  } else {
    Serial.println("   ✓ Reading OK");
  }
  
  Serial.println();
}

// ═══════════════════════════════════════════════════════════
// CURRENT SENSOR TEST
// ═══════════════════════════════════════════════════════════
void testCurrentSensor() {
  Serial.println("2. CURRENT SENSOR (ACS712-30A)");
  Serial.println("   Pin: GPIO35");
  Serial.println("   ─────────────────────────────────");
  
  int rawADC = analogRead(CURRENT_PIN);
  float adcVoltage = (rawADC / 4095.0) * 3.3;
  float current = (adcVoltage - ACS712_OFFSET) / ACS712_SENSITIVITY;
  
  if (current < 0) current = 0;  // No negative current
  
  Serial.print("   Raw ADC: ");
  Serial.print(rawADC);
  Serial.println(" (0-4095)");
  
  Serial.print("   ACS712 Output: ");
  Serial.print(adcVoltage, 3);
  Serial.println(" V");
  
  Serial.print("   Current: ");
  Serial.print(current, 3);
  Serial.println(" A");
  
  // Status check
  if (abs(adcVoltage - 2.5) < 0.1 && current < 0.1) {
    Serial.println("   ✓ Zero current (sensor at rest)");
  } else if (adcVoltage < 0.5 || adcVoltage > 4.5) {
    Serial.println("   ⚠️  WARNING: Output out of range!");
  } else {
    Serial.println("   ✓ Current flowing");
  }
  
  Serial.println();
}

// ═══════════════════════════════════════════════════════════
// LDR SENSOR TEST
// ═══════════════════════════════════════════════════════════
void testLDR() {
  Serial.println("3. IRRADIANCE SENSOR (LDR)");
  Serial.println("   Pin: GPIO32");
  Serial.println("   ─────────────────────────────────");
  
  int rawADC = analogRead(LDR_PIN);
  float adcVoltage = (rawADC / 4095.0) * 3.3;
  float irradiance = map(rawADC, 0, 4095, 0, 1200);
  
  Serial.print("   Raw ADC: ");
  Serial.print(rawADC);
  Serial.println(" (0-4095)");
  
  Serial.print("   LDR Voltage: ");
  Serial.print(adcVoltage, 3);
  Serial.println(" V");
  
  Serial.print("   Estimated Irradiance: ");
  Serial.print(irradiance, 0);
  Serial.println(" W/m²");
  
  // Light level indication
  if (rawADC < 500) {
    Serial.println("   💡 Very Dark");
  } else if (rawADC < 1500) {
    Serial.println("   🌙 Low Light");
  } else if (rawADC < 3000) {
    Serial.println("   ☁️  Moderate Light");
  } else {
    Serial.println("   ☀️  Bright Light");
  }
  
  Serial.println("   ⚠️  Needs calibration for accurate W/m²");
  Serial.println();
}

// ═══════════════════════════════════════════════════════════
// TEMPERATURE SENSORS TEST
// ═══════════════════════════════════════════════════════════
void testTemperatureSensors() {
  Serial.println("4. TEMPERATURE SENSORS (LM35)");
  Serial.println("   ─────────────────────────────────");
  
  // Panel Temperature
  int panelADC = analogRead(PANEL_TEMP_PIN);
  float panelVoltage = (panelADC / 4095.0) * 3.3;
  float panelTemp = panelVoltage * 100.0 * 2.0;  // x2 for voltage divider
  
  Serial.println("   A) Panel Temperature (GPIO33):");
  Serial.print("      Raw ADC: ");
  Serial.println(panelADC);
  Serial.print("      Voltage: ");
  Serial.print(panelVoltage, 3);
  Serial.println(" V");
  Serial.print("      Temperature: ");
  Serial.print(panelTemp, 1);
  Serial.println(" °C");
  
  if (panelTemp < 0 || panelTemp > 100) {
    Serial.println("      ⚠️  WARNING: Reading out of range!");
  } else {
    Serial.println("      ✓ Reading OK");
  }
  
  Serial.println();
  
  // Ambient Temperature
  int ambientADC = analogRead(AMBIENT_TEMP_PIN);
  float ambientVoltage = (ambientADC / 4095.0) * 3.3;
  float ambientTemp = ambientVoltage * 100.0 * 2.0;
  
  Serial.println("   B) Ambient Temperature (GPIO25):");
  Serial.print("      Raw ADC: ");
  Serial.println(ambientADC);
  Serial.print("      Voltage: ");
  Serial.print(ambientVoltage, 3);
  Serial.println(" V");
  Serial.print("      Temperature: ");
  Serial.print(ambientTemp, 1);
  Serial.println(" °C");
  
  if (ambientTemp < 0 || ambientTemp > 100) {
    Serial.println("      ⚠️  WARNING: Reading out of range!");
  } else {
    Serial.println("      ✓ Reading OK");
  }
  
  Serial.println();
}
