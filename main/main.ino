/*
 * IoT-Based Solar Panel Performance Monitoring System
 * 
 * This system monitors solar panel performance by measuring:
 * - Electrical output (voltage, current, power)
 * - Environmental conditions (irradiance, temperature)
 * - Efficiency (actual vs expected performance)
 * 
 * Data is uploaded to ThingSpeak cloud platform for visualization
 * Optional: Solar tracking using servo motors
 * 
 * Hardware: ESP32, ACS712, LM35, LDR, LCD I2C, Servos (optional)
 * Platform: Arduino IDE with ESP32 board package
 * 
 * Author: Your Team Name
 * Date: March 2025
 * License: MIT
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "config.h"

// ============================================================
// GLOBAL OBJECTS
// ============================================================

// WiFi and MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// LCD Display
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Servo Motors (if tracking enabled)
Servo servoAzimuth;
Servo servoElevation;

// ============================================================
// GLOBAL VARIABLES
// ============================================================

// Sensor Readings (Raw)
int voltageRaw = 0;
int currentRaw = 0;
int ldrRaw = 0;
int panelTempRaw = 0;
int ambientTempRaw = 0;

// Calculated Values
float voltage = 0.0;
float current = 0.0;
float power = 0.0;
float irradiance = 0.0;
float panelTemp = 0.0;
float ambientTemp = 0.0;
float expectedPower = 0.0;
float efficiency = 0.0;

// Moving Average Filters
float voltageAvg = 0.0;
float currentAvg = 0.0;

// Servo Positions (if tracking enabled)
int azimuthPos = SERVO_AZIMUTH_INIT;
int elevationPos = SERVO_ELEVATION_INIT;

// MPPT Variables (if MPPT enabled)
#if ENABLE_MPPT
int mpptDutyCycle = MPPT_INITIAL_DUTY;
float mpptPreviousPower = 0.0;
float mpptPreviousVoltage = 0.0;
float mpptPreviousCurrent = 0.0;
float mpptMaxPower = 0.0;
float mpptEfficiency = 0.0;
unsigned long lastMPPTUpdate = 0;
#endif

// Timing Variables
unsigned long lastUploadTime = 0;
unsigned long lastLCDUpdate = 0;
unsigned long lastTrackingUpdate = 0;
int lcdScreenMode = 0;

// ============================================================
// SETUP FUNCTION
// ============================================================

void setup() {
  // Initialize Serial Monitor
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  Serial.println("\n\n========================================");
  Serial.println("Solar Panel Monitoring System");
  Serial.println("========================================\n");
  
  // Configure ADC
  analogReadResolution(ADC_RESOLUTION);
  analogSetAttenuation(ADC_11db);  // 0-3.3V range
  
  // Configure GPIO Pins
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(CURRENT_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(PANEL_TEMP_PIN, INPUT);
  pinMode(AMBIENT_TEMP_PIN, INPUT);
  
  #if ENABLE_SOLAR_TRACKING
  pinMode(LDR_TOP_LEFT, INPUT);
  pinMode(LDR_TOP_RIGHT, INPUT);
  pinMode(LDR_BOTTOM_LEFT, INPUT);
  pinMode(LDR_BOTTOM_RIGHT, INPUT);
  #endif
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize LCD
  Serial.println("Initializing LCD...");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Solar Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  
  // Initialize Servos (if tracking enabled)
  #if ENABLE_SOLAR_TRACKING
  Serial.println("Initializing servos...");
  servoAzimuth.attach(SERVO_AZIMUTH_PIN);
  servoElevation.attach(SERVO_ELEVATION_PIN);
  servoAzimuth.write(azimuthPos);
  servoElevation.write(elevationPos);
  delay(1000);
  #endif
  
  // Initialize MPPT (if enabled)
  #if ENABLE_MPPT
  Serial.println("Initializing MPPT controller...");
  ledcSetup(0, MPPT_PWM_FREQ, MPPT_PWM_RESOLUTION);  // Configure PWM channel 0
  ledcAttachPin(MPPT_PWM_PIN, 0);                     // Attach to pin
  ledcWrite(0, mpptDutyCycle);                        // Set initial duty cycle
  Serial.print("MPPT PWM initialized at ");
  Serial.print((mpptDutyCycle * 100.0) / 1023.0);
  Serial.println("% duty cycle");
  delay(500);
  #endif
  
  // Connect to WiFi
  connectWiFi();
  
  // Setup MQTT
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  
  // Initial sensor reading
  readSensors();
  
  Serial.println("\nSystem Ready!\n");
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop() {
  unsigned long currentTime = millis();
  
  // Read all sensors
  readSensors();
  
  // Calculate performance metrics
  calculatePerformance();
  
  // Update LCD display (rotating screens)
  if (currentTime - lastLCDUpdate >= LCD_UPDATE_INTERVAL) {
    updateLCD();
    lastLCDUpdate = currentTime;
  }
  
  // Print data to Serial Monitor
  printData();
  
  // Upload to ThingSpeak
  if (currentTime - lastUploadTime >= UPLOAD_INTERVAL) {
    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();
    sendToThingSpeak();
    lastUploadTime = currentTime;
  }
  
  // Solar tracking (if enabled)
  #if ENABLE_SOLAR_TRACKING
  if (currentTime - lastTrackingUpdate >= TRACKING_INTERVAL) {
    trackSun();
    lastTrackingUpdate = currentTime;
  }
  #endif
  
  // MPPT algorithm (if enabled)
  #if ENABLE_MPPT
  if (currentTime - lastMPPTUpdate >= MPPT_UPDATE_INTERVAL) {
    runMPPT();
    lastMPPTUpdate = currentTime;
  }
  #endif
  
  // Main loop delay
  delay(1000);
}

// ============================================================
// SENSOR READING FUNCTIONS
// ============================================================

void readSensors() {
  // Read raw ADC values
  voltageRaw = analogRead(VOLTAGE_PIN);
  currentRaw = analogRead(CURRENT_PIN);
  ldrRaw = analogRead(LDR_PIN);
  panelTempRaw = analogRead(PANEL_TEMP_PIN);
  ambientTempRaw = analogRead(AMBIENT_TEMP_PIN);
  
  // Convert Voltage (ADC → Volts)
  float vAdc = (voltageRaw / (float)ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE;
  voltage = vAdc * VOLTAGE_DIVIDER_RATIO;
  
  // Convert Current (ACS712)
  float iAdc = (currentRaw / (float)ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE;
  current = (iAdc - ACS712_OFFSET) / ACS712_SENSITIVITY;
  if (current < 0) current = 0;  // No negative current
  
  // Calculate Power
  power = voltage * current;
  
  // Convert LDR to Irradiance (W/m²)
  // Linear mapping - needs calibration for your location
  irradiance = map(ldrRaw, LDR_MIN_ADC, LDR_MAX_ADC, 0, IRRADIANCE_MAX);
  if (irradiance < 0) irradiance = 0;
  if (irradiance > IRRADIANCE_MAX) irradiance = IRRADIANCE_MAX;
  
  // Convert Panel Temperature (LM35)
  float panelVolt = (panelTempRaw / (float)ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE;
  panelTemp = panelVolt * 100.0 * LM35_VOLTAGE_DIVIDER;
  
  // Convert Ambient Temperature (LM35)
  float ambientVolt = (ambientTempRaw / (float)ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE;
  ambientTemp = ambientVolt * 100.0 * LM35_VOLTAGE_DIVIDER;
  
  // Apply moving average filter for stability
  #if USE_MOVING_AVERAGE
  voltageAvg = MOVING_AVERAGE_ALPHA * voltage + (1 - MOVING_AVERAGE_ALPHA) * voltageAvg;
  currentAvg = MOVING_AVERAGE_ALPHA * current + (1 - MOVING_AVERAGE_ALPHA) * currentAvg;
  voltage = voltageAvg;
  current = currentAvg;
  power = voltage * current;  // Recalculate with filtered values
  #endif
  
  // Debug output
  #if DEBUG_SENSORS
  Serial.println("=== RAW SENSOR READINGS ===");
  Serial.print("Voltage ADC: "); Serial.println(voltageRaw);
  Serial.print("Current ADC: "); Serial.println(currentRaw);
  Serial.print("LDR ADC: "); Serial.println(ldrRaw);
  Serial.print("Panel Temp ADC: "); Serial.println(panelTempRaw);
  Serial.print("Ambient Temp ADC: "); Serial.println(ambientTempRaw);
  Serial.println("===========================\n");
  #endif
}

// ============================================================
// PERFORMANCE CALCULATION
// ============================================================

void calculatePerformance() {
  // Temperature correction factor
  // P_temp = P_STC × [1 + β(T_panel - T_STC)]
  float tempCorrection = 1.0 + PANEL_TEMP_COEFFICIENT * (panelTemp - STC_TEMPERATURE);
  
  // Expected Power Calculation
  // P_expected = (G / G_STC) × P_rated × Temp_correction
  // Or more precisely: P = G × A × η × Temp_correction
  
  if (irradiance > 10) {  // Only calculate if there's meaningful sunlight
    expectedPower = (irradiance / 1000.0) * PANEL_AREA * PANEL_EFFICIENCY * 1000.0 * tempCorrection;
  } else {
    expectedPower = 0.0;
  }
  
  // Performance Ratio (Efficiency)
  if (expectedPower > 0.5) {  // Avoid division by very small numbers
    efficiency = (power / expectedPower) * 100.0;
    
    // Limit to realistic range
    if (efficiency > 100.0) efficiency = 100.0;
    if (efficiency < 0.0) efficiency = 0.0;
  } else {
    efficiency = 0.0;
  }
  
  // Safety checks
  if (panelTemp > MAX_PANEL_TEMP) {
    Serial.println("⚠️ WARNING: Panel temperature too high!");
  }
  
  if (efficiency < EFFICIENCY_LOW_THRESHOLD && irradiance > 500) {
    Serial.println("⚠️ WARNING: Low efficiency detected!");
  }
}

// ============================================================
// LCD DISPLAY FUNCTIONS
// ============================================================

void updateLCD() {
  // Cycle through different screens
  lcdScreenMode = (lcdScreenMode + 1) % 3;
  
  lcd.clear();
  
  switch(lcdScreenMode) {
    case 0:  // Electrical parameters
      lcd.setCursor(0, 0);
      lcd.print("V:");
      lcd.print(voltage, 1);
      lcd.print("V I:");
      lcd.print(current, 2);
      lcd.print("A");
      
      lcd.setCursor(0, 1);
      lcd.print("Power: ");
      lcd.print(power, 2);
      lcd.print(" W");
      break;
      
    case 1:  // Environmental conditions
      lcd.setCursor(0, 0);
      lcd.print("Irr:");
      lcd.print(irradiance, 0);
      lcd.print(" W/m2");
      
      lcd.setCursor(0, 1);
      lcd.print("Tp:");
      lcd.print(panelTemp, 1);
      lcd.print(" Ta:");
      lcd.print(ambientTemp, 1);
      break;
      
    case 2:  // Performance metrics
      lcd.setCursor(0, 0);
      lcd.print("Actual:");
      lcd.print(power, 1);
      lcd.print("W");
      
      lcd.setCursor(0, 1);
      lcd.print("Eff:");
      lcd.print(efficiency, 1);
      lcd.print("% E:");
      lcd.print(expectedPower, 1);
      break;
  }
}

// ============================================================
// SERIAL MONITOR OUTPUT
// ============================================================

void printData() {
  #if DEBUG_MODE
  Serial.println("\n========== SOLAR MONITOR ==========");
  Serial.print("Voltage: "); Serial.print(voltage, 2); Serial.println(" V");
  Serial.print("Current: "); Serial.print(current, 3); Serial.println(" A");
  Serial.print("Power: "); Serial.print(power, 2); Serial.println(" W");
  Serial.println("-----------------------------------");
  Serial.print("Irradiance: "); Serial.print(irradiance, 0); Serial.println(" W/m²");
  Serial.print("Panel Temp: "); Serial.print(panelTemp, 1); Serial.println(" °C");
  Serial.print("Ambient Temp: "); Serial.print(ambientTemp, 1); Serial.println(" °C");
  Serial.println("-----------------------------------");
  Serial.print("Expected Power: "); Serial.print(expectedPower, 2); Serial.println(" W");
  Serial.print("Efficiency: "); Serial.print(efficiency, 1); Serial.println(" %");
  Serial.println("===================================\n");
  #endif
}

// ============================================================
// WiFi CONNECTION
// ============================================================

void connectWiFi() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(attempts % 16, 1);
    lcd.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected!");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(2000);
  } else {
    Serial.println("\n✗ WiFi Connection Failed!");
    Serial.println("Check SSID and password in config.h");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed!");
    lcd.setCursor(0, 1);
    lcd.print("Check config.h");
    delay(3000);
  }
}

// ============================================================
// MQTT / THINGSPEAK FUNCTIONS
// ============================================================

void reconnectMQTT() {
  int attempts = 0;
  
  while (!mqttClient.connected() && attempts < 3) {
    Serial.print("Connecting to MQTT...");
    
    #if DEBUG_MQTT
    Serial.print("\nServer: "); Serial.println(MQTT_SERVER);
    Serial.print("Port: "); Serial.println(MQTT_PORT);
    Serial.print("Username: "); Serial.println(THINGSPEAK_MQTT_USERNAME);
    #endif
    
    // Attempt connection
    String clientID = "ESP32Solar-" + String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientID.c_str(), 
                           THINGSPEAK_MQTT_USERNAME, 
                           THINGSPEAK_MQTT_PASSWORD)) {
      Serial.println(" ✓ Connected");
      return;
    } else {
      Serial.print(" ✗ Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5s...");
      delay(5000);
      attempts++;
    }
  }
  
  if (!mqttClient.connected()) {
    Serial.println("⚠️ MQTT connection failed after 3 attempts");
  }
}

void sendToThingSpeak() {
  if (!mqttClient.connected()) {
    Serial.println("⚠️ MQTT not connected, skipping upload");
    return;
  }
  
  // Create MQTT topic
  String topic = "channels/" + String(THINGSPEAK_CHANNEL_ID) + "/publish";
  
  // Create payload with all 8 fields
  String payload = "field1=" + String(voltage, 2) +
                   "&field2=" + String(current, 3) +
                   "&field3=" + String(power, 2) +
                   "&field4=" + String(irradiance, 0) +
                   "&field5=" + String(panelTemp, 1) +
                   "&field6=" + String(expectedPower, 2) +
                   "&field7=" + String(efficiency, 1) +
                   "&field8=" + String(ambientTemp, 1);
  
  #if DEBUG_MQTT
  Serial.println("=== MQTT PUBLISH ===");
  Serial.print("Topic: "); Serial.println(topic);
  Serial.print("Payload: "); Serial.println(payload);
  Serial.println("===================");
  #endif
  
  // Publish to ThingSpeak
  if (mqttClient.publish(topic.c_str(), payload.c_str())) {
    Serial.println("✓ Data sent to ThingSpeak");
  } else {
    Serial.println("✗ Failed to send data");
  }
}

// ============================================================
// SOLAR TRACKING FUNCTIONS (Optional)
// ============================================================

#if ENABLE_SOLAR_TRACKING

void trackSun() {
  #if TRACKING_METHOD == 1
  // Light-based tracking using quad LDR sensors
  trackSunByLight();
  #elif TRACKING_METHOD == 2
  // Time-based tracking using solar position algorithm
  trackSunByTime();
  #endif
}

void trackSunByLight() {
  // Read all 4 LDR sensors
  int topLeft = analogRead(LDR_TOP_LEFT);
  int topRight = analogRead(LDR_TOP_RIGHT);
  int bottomLeft = analogRead(LDR_BOTTOM_LEFT);
  int bottomRight = analogRead(LDR_BOTTOM_RIGHT);
  
  // Calculate averages
  int avgTop = (topLeft + topRight) / 2;
  int avgBottom = (bottomLeft + bottomRight) / 2;
  int avgLeft = (topLeft + bottomLeft) / 2;
  int avgRight = (topRight + bottomRight) / 2;
  
  #if DEBUG_TRACKING
  Serial.println("=== TRACKING DEBUG ===");
  Serial.print("TL:"); Serial.print(topLeft);
  Serial.print(" TR:"); Serial.print(topRight);
  Serial.print(" BL:"); Serial.print(bottomLeft);
  Serial.print(" BR:"); Serial.println(bottomRight);
  #endif
  
  // Vertical tracking (Elevation) - for dual-axis only
  #if TRACKING_MODE == 2
  int vertDiff = avgTop - avgBottom;
  if (abs(vertDiff) > TRACKING_THRESHOLD) {
    if (vertDiff > 0) {
      // Top is brighter → tilt up
      elevationPos = constrain(elevationPos + SERVO_STEP, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    } else {
      // Bottom is brighter → tilt down
      elevationPos = constrain(elevationPos - SERVO_STEP, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    }
    servoElevation.write(elevationPos);
    
    #if DEBUG_TRACKING
    Serial.print("Elevation adjusted to: "); Serial.println(elevationPos);
    #endif
  }
  #endif
  
  // Horizontal tracking (Azimuth)
  int horizDiff = avgLeft - avgRight;
  if (abs(horizDiff) > TRACKING_THRESHOLD) {
    if (horizDiff > 0) {
      // Left is brighter → rotate left
      azimuthPos = constrain(azimuthPos - SERVO_STEP, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    } else {
      // Right is brighter → rotate right
      azimuthPos = constrain(azimuthPos + SERVO_STEP, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    }
    servoAzimuth.write(azimuthPos);
    
    #if DEBUG_TRACKING
    Serial.print("Azimuth adjusted to: "); Serial.println(azimuthPos);
    #endif
  }
  
  #if DEBUG_TRACKING
  Serial.println("=====================");
  #endif
}

void trackSunByTime() {
  // TODO: Implement astronomical solar position calculation
  // This requires NTP time sync and solar position algorithm
  // For now, using simplified time-based approach
  
  // Get current time (you need to implement NTP sync first)
  // For demo purposes, using a simplified linear movement
  
  Serial.println("⚠️ Time-based tracking not fully implemented");
  Serial.println("Use light-based tracking (TRACKING_METHOD = 1) instead");
}

#endif  // ENABLE_SOLAR_TRACKING

// ============================================================
// MPPT FUNCTIONS (Maximum Power Point Tracking)
// ============================================================

#if ENABLE_MPPT

void runMPPT() {
  // Calculate current power
  float currentPower = voltage * current;
  
  // Safety checks
  if (voltage < MPPT_MIN_VOLTAGE || voltage > MPPT_MAX_VOLTAGE) {
    Serial.println("⚠️ MPPT: Voltage out of safe range!");
    return;
  }
  
  if (current > MPPT_MAX_CURRENT) {
    Serial.println("⚠️ MPPT: Current limit exceeded!");
    mpptDutyCycle = constrain(mpptDutyCycle - MPPT_STEP_SIZE * 2, MPPT_MIN_DUTY, MPPT_MAX_DUTY);
    ledcWrite(0, mpptDutyCycle);
    return;
  }
  
  #if MPPT_METHOD == 1
  // Perturb & Observe Algorithm
  mpptPerturbAndObserve(currentPower);
  
  #elif MPPT_METHOD == 2
  // Incremental Conductance Algorithm
  mpptIncrementalConductance();
  #endif
  
  // Update MPPT efficiency
  if (irradiance > 100 && expectedPower > 0.5) {
    mpptMaxPower = max(mpptMaxPower, currentPower);
    mpptEfficiency = (currentPower / expectedPower) * 100.0;
  }
  
  // Apply duty cycle
  ledcWrite(0, mpptDutyCycle);
  
  // Store values for next iteration
  mpptPreviousPower = currentPower;
  mpptPreviousVoltage = voltage;
  mpptPreviousCurrent = current;
  
  #if MPPT_ENABLE_LOGGING
  static unsigned long lastLog = 0;
  if (millis() - lastLog > 5000) {  // Log every 5 seconds
    Serial.println("\n=== MPPT STATUS ===");
    Serial.print("Duty Cycle: ");
    Serial.print((mpptDutyCycle * 100.0) / 1023.0, 1);
    Serial.println("%");
    Serial.print("Current Power: ");
    Serial.print(currentPower, 2);
    Serial.println(" W");
    Serial.print("Max Power (session): ");
    Serial.print(mpptMaxPower, 2);
    Serial.println(" W");
    Serial.print("MPPT Efficiency: ");
    Serial.print(mpptEfficiency, 1);
    Serial.println("%");
    Serial.println("==================\n");
    lastLog = millis();
  }
  #endif
}

// Perturb & Observe MPPT Algorithm
void mpptPerturbAndObserve(float currentPower) {
  float powerChange = currentPower - mpptPreviousPower;
  float voltageChange = voltage - mpptPreviousVoltage;
  
  // First iteration - just perturb
  if (mpptPreviousPower == 0) {
    mpptDutyCycle += MPPT_STEP_SIZE;
    return;
  }
  
  // P&O Logic:
  // If power increased and voltage increased → increase duty (decrease voltage)
  // If power increased and voltage decreased → decrease duty (increase voltage)
  // If power decreased and voltage increased → decrease duty (increase voltage)
  // If power decreased and voltage decreased → increase duty (decrease voltage)
  
  if (powerChange > 0.01) {  // Power increased
    if (voltageChange > 0) {
      // Power↑ Voltage↑ → Operating left of MPP → Decrease voltage (increase duty)
      mpptDutyCycle += MPPT_STEP_SIZE;
    } else {
      // Power↑ Voltage↓ → Operating right of MPP → Increase voltage (decrease duty)
      mpptDutyCycle -= MPPT_STEP_SIZE;
    }
  } else if (powerChange < -0.01) {  // Power decreased
    if (voltageChange > 0) {
      // Power↓ Voltage↑ → Was right of MPP → Go back (decrease duty)
      mpptDutyCycle -= MPPT_STEP_SIZE;
    } else {
      // Power↓ Voltage↓ → Was left of MPP → Go back (increase duty)
      mpptDutyCycle += MPPT_STEP_SIZE;
    }
  }
  // If power change is negligible, we're at MPP - make small perturbation
  else {
    // Small perturbation to check if still at MPP
    static bool perturbDirection = true;
    if (perturbDirection) {
      mpptDutyCycle += MPPT_STEP_SIZE / 2;
    } else {
      mpptDutyCycle -= MPPT_STEP_SIZE / 2;
    }
    perturbDirection = !perturbDirection;
  }
  
  // Constrain duty cycle to safe limits
  mpptDutyCycle = constrain(mpptDutyCycle, MPPT_MIN_DUTY, MPPT_MAX_DUTY);
}

// Incremental Conductance MPPT Algorithm
void mpptIncrementalConductance() {
  float dV = voltage - mpptPreviousVoltage;
  float dI = current - mpptPreviousCurrent;
  
  // Avoid division by zero
  if (abs(dV) < 0.01) {
    // Voltage hasn't changed - check if at MPP
    if (abs(dI) < 0.001) {
      // At MPP - no change needed
      return;
    } else if (dI > 0) {
      // Current increasing → increase duty
      mpptDutyCycle += MPPT_STEP_SIZE;
    } else {
      // Current decreasing → decrease duty
      mpptDutyCycle -= MPPT_STEP_SIZE;
    }
    return;
  }
  
  // Calculate conductance (I/V) and incremental conductance (dI/dV)
  float conductance = current / voltage;
  float incrementalConductance = dI / dV;
  
  // InCond Algorithm:
  // At MPP: dI/dV = -I/V
  // Left of MPP: dI/dV > -I/V → increase voltage (decrease duty)
  // Right of MPP: dI/dV < -I/V → decrease voltage (increase duty)
  
  float difference = incrementalConductance + conductance;
  
  if (abs(difference) < 0.001) {
    // At MPP - maintain duty cycle
    return;
  } else if (difference > 0) {
    // Left of MPP → increase voltage (decrease duty)
    mpptDutyCycle -= MPPT_STEP_SIZE;
  } else {
    // Right of MPP → decrease voltage (increase duty)
    mpptDutyCycle += MPPT_STEP_SIZE;
  }
  
  // Constrain duty cycle
  mpptDutyCycle = constrain(mpptDutyCycle, MPPT_MIN_DUTY, MPPT_MAX_DUTY);
}

#endif  // ENABLE_MPPT
