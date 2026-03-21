/*
 * Configuration File for Solar Panel Monitoring System
 * 
 * INSTRUCTIONS:
 * 1. Fill in your WiFi credentials
 * 2. Add your ThingSpeak API keys
 * 3. Adjust solar panel specifications
 * 4. Configure sensor calibration values
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// WiFi CONFIGURATION
// ============================================================
#define WIFI_SSID "YOUR_WIFI_SSID"          // Replace with your WiFi network name
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"  // Replace with your WiFi password

// ============================================================
// THINGSPEAK CONFIGURATION
// ============================================================
// Get these from: https://thingspeak.com/channels/YOUR_CHANNEL/api_keys
#define THINGSPEAK_CHANNEL_ID "YOUR_CHANNEL_ID"              // e.g., "1234567"
#define THINGSPEAK_WRITE_API_KEY "YOUR_WRITE_API_KEY"        // 16 character key
#define THINGSPEAK_MQTT_USERNAME "YOUR_MQTT_USERNAME"        // From ThingSpeak account
#define THINGSPEAK_MQTT_PASSWORD "YOUR_MQTT_CLIENT_ID"       // MQTT API Key

// MQTT Broker Settings
#define MQTT_SERVER "mqtt3.thingspeak.com"
#define MQTT_PORT 1883

// Data Upload Interval (milliseconds)
#define UPLOAD_INTERVAL 15000  // Upload every 15 seconds (ThingSpeak minimum)

// ============================================================
// PIN CONFIGURATION
// ============================================================

// Analog Input Pins (ADC)
#define VOLTAGE_PIN 34        // GPIO34 - Panel Voltage (via voltage divider)
#define CURRENT_PIN 35        // GPIO35 - ACS712 Current Sensor output
#define LDR_PIN 32            // GPIO32 - LDR for irradiance measurement
#define PANEL_TEMP_PIN 33     // GPIO33 - LM35 Panel Temperature
#define AMBIENT_TEMP_PIN 25   // GPIO25 - LM35 Ambient Temperature

// I2C Pins (Display)
#define SDA_PIN 21            // GPIO21 - I2C Data
#define SCL_PIN 22            // GPIO22 - I2C Clock

// Servo Pins (Optional - for solar tracking)
#define SERVO_AZIMUTH_PIN 18  // GPIO18 - East-West rotation
#define SERVO_ELEVATION_PIN 19 // GPIO19 - Up-Down tilt

// LDR Tracking Sensors (Optional - for quad sensor tracking)
#define LDR_TOP_LEFT 26       // GPIO26
#define LDR_TOP_RIGHT 27      // GPIO27
#define LDR_BOTTOM_LEFT 14    // GPIO14
#define LDR_BOTTOM_RIGHT 12   // GPIO12

// ============================================================
// LCD CONFIGURATION
// ============================================================
#define LCD_I2C_ADDRESS 0x27  // Try 0x3F if this doesn't work
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// ============================================================
// SENSOR CALIBRATION
// ============================================================

// Voltage Divider Configuration
// Circuit: 100kΩ and 10kΩ resistors
#define VOLTAGE_DIVIDER_RATIO 11.0  // (100k + 10k) / 10k
#define VOLTAGE_MAX 25.0             // Maximum measurable voltage

// ACS712 Current Sensor Configuration
#define ACS712_SENSITIVITY 0.066  // 66mV per Amp for 30A version
                                  // Use 0.100 for 20A version
                                  // Use 0.185 for 5A version
#define ACS712_OFFSET 2.5         // Output voltage at 0A
#define CURRENT_MAX 30.0          // Maximum current (30A for ACS712-30A)

// LDR Calibration (adjust after field testing)
#define LDR_SCALE_FACTOR 1.0      // Multiplier to convert ADC to W/m²
#define LDR_MIN_ADC 0             // ADC reading in complete darkness
#define LDR_MAX_ADC 4095          // ADC reading in bright sunlight
#define IRRADIANCE_MAX 1200.0     // Maximum irradiance in W/m² (Egypt peak)

// Temperature Sensor Configuration (LM35)
#define LM35_VOLTAGE_DIVIDER 2.0  // Voltage divider ratio (5V→3.3V)
#define LM35_MV_PER_DEGREE 10.0   // LM35 outputs 10mV per °C

// ============================================================
// SOLAR PANEL SPECIFICATIONS
// ============================================================
// Adjust these values according to your solar panel datasheet

#define PANEL_RATED_POWER 10.0        // Rated power in Watts
#define PANEL_RATED_VOLTAGE 18.0      // Rated voltage in Volts
#define PANEL_RATED_CURRENT 0.56      // Rated current in Amps
#define PANEL_AREA 0.072              // Panel area in m² (e.g., 30cm x 24cm)
#define PANEL_EFFICIENCY 0.15         // Panel efficiency (15% typical)
#define PANEL_TEMP_COEFFICIENT -0.004 // Temperature coefficient (%/°C)
                                       // Typical: -0.4% per °C for silicon
#define STC_TEMPERATURE 25.0          // Standard Test Condition temp (°C)
#define STC_IRRADIANCE 1000.0         // Standard Test Condition irradiance (W/m²)

// ============================================================
// MPPT (Maximum Power Point Tracking) CONFIGURATION
// ============================================================

// Enable/Disable MPPT
#define ENABLE_MPPT false             // Set to true to enable MPPT
#define MPPT_METHOD 1                 // 1 = Perturb & Observe, 2 = Incremental Conductance

// MPPT Hardware (PWM-based DC-DC converter control)
#define MPPT_PWM_PIN 13               // GPIO13 - PWM output to DC-DC converter
#define MPPT_PWM_FREQ 25000           // 25kHz PWM frequency (typical for MPPT)
#define MPPT_PWM_RESOLUTION 10        // 10-bit resolution (0-1023)

// MPPT Algorithm Parameters
#define MPPT_UPDATE_INTERVAL 100      // Update MPPT every 100ms
#define MPPT_STEP_SIZE 5              // Duty cycle step size (0-1023)
#define MPPT_VOLTAGE_STEP 0.5         // Voltage perturbation (V)
#define MPPT_INITIAL_DUTY 512         // Initial PWM duty cycle (50%)

// MPPT Limits
#define MPPT_MIN_DUTY 100             // Minimum duty cycle (~10%)
#define MPPT_MAX_DUTY 900             // Maximum duty cycle (~90%)
#define MPPT_MIN_VOLTAGE 10.0         // Minimum panel voltage (V)
#define MPPT_MAX_VOLTAGE 22.0         // Maximum panel voltage (V)
#define MPPT_MAX_CURRENT 2.0          // Maximum current limit (A)

// MPPT Efficiency Tracking
#define MPPT_ENABLE_LOGGING true      // Log MPPT performance

// ============================================================
// SOLAR TRACKING CONFIGURATION (Optional)
// ============================================================

// Enable/Disable Features
#define ENABLE_SOLAR_TRACKING false   // Set to true to enable tracking
#define TRACKING_MODE 1               // 1 = Single-axis, 2 = Dual-axis
#define TRACKING_METHOD 1             // 1 = Light-based, 2 = Time-based

// Tracking Parameters
#define TRACKING_INTERVAL 5000        // Update tracking every 5 seconds
#define TRACKING_THRESHOLD 50         // ADC difference to trigger movement
#define SERVO_STEP 2                  // Degrees per adjustment
#define SERVO_MIN_ANGLE 0             // Minimum servo angle
#define SERVO_MAX_ANGLE 180           // Maximum servo angle

// Initial Servo Positions
#define SERVO_AZIMUTH_INIT 90         // Center position for azimuth
#define SERVO_ELEVATION_INIT 45       // Initial tilt angle

// ============================================================
// SYSTEM SETTINGS
// ============================================================

// ADC Configuration
#define ADC_RESOLUTION 12             // ESP32 ADC resolution (12-bit = 0-4095)
#define ADC_MAX_VALUE 4095            // Maximum ADC value
#define ADC_REFERENCE_VOLTAGE 3.3     // ESP32 ADC reference voltage

// Data Filtering
#define USE_MOVING_AVERAGE true       // Enable moving average filter
#define MOVING_AVERAGE_ALPHA 0.2      // Filter coefficient (0.1-0.3)

// Display Settings
#define LCD_UPDATE_INTERVAL 3000      // LCD screen rotation interval (ms)
#define SERIAL_BAUD_RATE 115200       // Serial monitor baud rate

// Timeouts
#define WIFI_TIMEOUT 20000            // WiFi connection timeout (ms)
#define MQTT_TIMEOUT 10000            // MQTT connection timeout (ms)

// ============================================================
// LOCATION SETTINGS (For time-based tracking)
// ============================================================
// Get coordinates from: https://www.latlong.net/

#define LOCATION_LATITUDE 30.0444     // Alexandria, Egypt latitude
#define LOCATION_LONGITUDE 31.2357    // Alexandria, Egypt longitude
#define TIMEZONE_OFFSET 2             // Egypt is UTC+2

// NTP Server for time synchronization
#define NTP_SERVER "pool.ntp.org"

// ============================================================
// SAFETY LIMITS
// ============================================================

#define MAX_PANEL_TEMP 80.0           // Maximum safe panel temperature (°C)
#define MIN_PANEL_VOLTAGE 5.0         // Minimum expected voltage (fault detection)
#define MAX_PANEL_VOLTAGE 25.0        // Maximum expected voltage
#define EFFICIENCY_LOW_THRESHOLD 50.0 // Alert if efficiency below this %

// ============================================================
// DEBUG OPTIONS
// ============================================================

#define DEBUG_MODE true               // Enable debug output to Serial
#define DEBUG_SENSORS false           // Print raw sensor readings
#define DEBUG_MQTT false              // Print MQTT messages
#define DEBUG_TRACKING false          // Print tracking debug info

#endif // CONFIG_H
