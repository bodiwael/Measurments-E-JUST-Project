# Sensor Calibration Guide

Step-by-step procedures to calibrate all sensors for accurate measurements.

## 🎯 Why Calibrate?

- **Accuracy:** Sensors have manufacturing tolerances
- **Environment:** Local conditions affect readings
- **Precision:** Academic projects require accurate data
- **Reliability:** Proper calibration prevents false alarms

---

## 📋 Required Equipment

### Essential:
- ✅ Multimeter (DMM)
- ✅ Variable voltage source (0-25V) or known voltages
- ✅ Known current load (resistors)
- ✅ Thermometer (digital, ±0.5°C accuracy)
- ✅ Reference light source or smartphone lux meter app

### Optional but Helpful:
- Reference pyranometer (for irradiance)
- Oscilloscope (for noise analysis)
- Calibrated power supply
- Professional solar meter

---

## 1️⃣ Voltage Sensor Calibration

### Step 1: Prepare Test Setup
```
Power Supply (+) ──→ Voltage Divider ──→ ESP32 GPIO34
                                     └──→ Multimeter (+)
Power Supply (-) ──→ GND ────────────────→ Multimeter (-)
```

### Step 2: Test Multiple Points
Test at these voltages: 0V, 5V, 10V, 15V, 20V

For each voltage:
1. Set power supply voltage
2. Measure with multimeter (V_actual)
3. Read ESP32 Serial Monitor (V_measured)
4. Record in table:

| V_actual (Multimeter) | V_measured (ESP32) | Error (%) |
|-----------------------|--------------------|-----------|
| 0.00 V | ___ V | ___ % |
| 5.00 V | ___ V | ___ % |
| 10.00 V | ___ V | ___ % |
| 15.00 V | ___ V | ___ % |
| 20.00 V | ___ V | ___ % |

### Step 3: Calculate Calibration Factor
```
Calibration_Factor = Average(V_actual / V_measured)

Example:
If multimeter reads 12.0V and ESP32 reads 11.5V:
Factor = 12.0 / 11.5 = 1.043

Update in config.h:
#define VOLTAGE_DIVIDER_RATIO (11.0 * 1.043)  // = 11.473
```

### Step 4: Verify Accuracy
- Re-test all voltage points
- Error should be <2%
- If not, recheck wiring and resistor values

---

## 2️⃣ Current Sensor Calibration (ACS712)

### Step 1: Zero Current Calibration

With NO current flowing:
1. Power on system
2. Read ACS712 output voltage
3. Should be approximately 2.5V

If not 2.5V ±0.1V:
```cpp
// Update in config.h
#define ACS712_OFFSET 2.48  // Your measured value
```

### Step 2: Known Current Test

Create test load:
```
12V Power Supply ──→ 10Ω Resistor ──→ ACS712 ──→ Ground

Expected Current: I = V / R = 12V / 10Ω = 1.2A
```

1. Measure actual current with multimeter in series
2. Read ESP32 current value
3. Calculate correction:

```
Actual Current: 1.20A (multimeter)
Measured Current: 1.15A (ESP32)
Correction Factor: 1.20 / 1.15 = 1.043

Update in code:
current = ((adcVoltage - ACS712_OFFSET) / ACS712_SENSITIVITY) * 1.043;
```

### Step 3: Multiple Load Tests

| Load (Ω) | Expected I (A) | Measured I (A) | Error (%) |
|----------|----------------|----------------|-----------|
| 100Ω | 0.12A | ___ A | ___ % |
| 47Ω | 0.26A | ___ A | ___ % |
| 22Ω | 0.55A | ___ A | ___ % |
| 10Ω | 1.20A | ___ A | ___ % |

### Step 4: Sensitivity Adjustment

If errors are consistent across all currents:
```cpp
// ACS712-30A default: 66mV/A
// Adjust if needed (typically 64-68mV/A due to tolerance)

#define ACS712_SENSITIVITY 0.067  // Increase if readings are low
                                  // Decrease if readings are high
```

---

## 3️⃣ Temperature Sensor Calibration (LM35)

### Step 1: Room Temperature Test

1. Measure room temperature with reference thermometer
2. Wait for LM35 to stabilize (5 minutes)
3. Compare readings:

```
Reference Thermometer: ___°C
LM35 Reading: ___°C
Offset: Reference - LM35 = ___°C
```

### Step 2: Ice Water Test (0°C)

1. Prepare ice water bath (crushed ice + water)
2. Submerge LM35 (sealed in plastic bag)
3. Wait 2 minutes for stabilization
4. Should read ~0°C

### Step 3: Hot Water Test (50-60°C)

1. Heat water to known temperature (use thermometer)
2. Submerge sealed LM35
3. Wait 2 minutes
4. Compare readings

### Step 4: Create Calibration Table

| Test | Reference (°C) | LM35 (°C) | Offset (°C) |
|------|----------------|-----------|-------------|
| Ice Water | 0.0 | ___ | ___ |
| Room Temp | ___ | ___ | ___ |
| Hot Water | ___ | ___ | ___ |

### Step 5: Apply Correction

```cpp
// If consistent offset (e.g., always 2°C high)
float tempCorrection = -2.0;
panelTemp = (panelVolt * 100.0 * 2.0) + tempCorrection;

// If scaling issue (slope error)
float tempScale = 1.05;  // Adjust based on your tests
panelTemp = (panelVolt * 100.0 * 2.0) * tempScale;
```

---

## 4️⃣ Irradiance Sensor Calibration (LDR)

### Step 1: Dark Baseline

1. Cover LDR completely (box, tape)
2. Read ADC value (should be near 0)
3. Record: `LDR_MIN_ADC = ___`

### Step 2: Maximum Light

1. Direct sunlight at solar noon (12:00-13:00)
2. Point LDR directly at sun
3. Read ADC value (should be near 4095)
4. Record: `LDR_MAX_ADC = ___`

### Step 3: Reference Comparison Method A (Weather Data)

On a clear sunny day:
1. Check local weather service for solar irradiance
2. Egypt typical peak: 900-1100 W/m²
3. Read LDR ADC at same time
4. Calculate scale factor:

```
Peak Irradiance: 1000 W/m² (from weather service)
LDR ADC at peak: 3800
Scale = 1000 / 3800 = 0.263 W/m² per ADC unit

Update in code:
irradiance = ldrRaw * 0.263;
```

### Step 4: Reference Comparison Method B (Solar Panel Method)

On clear day with clean panel:
1. Measure panel voltage and current
2. Calculate actual power: P = V × I
3. Estimate irradiance from panel performance:

```
Panel Power: 8.5W
Panel Area: 0.072 m²
Panel Efficiency: 15%

Estimated Irradiance = P / (Area × Efficiency)
                     = 8.5 / (0.072 × 0.15)
                     = 787 W/m²

LDR ADC Reading: 3200
Scale = 787 / 3200 = 0.246 W/m² per ADC unit
```

### Step 5: Multi-Point Calibration

Test at different times of day:

| Time | Weather | LDR ADC | Estimated W/m² |
|------|---------|---------|----------------|
| 8:00 AM | Clear | ___ | ~400 W/m² |
| 10:00 AM | Clear | ___ | ~700 W/m² |
| 12:00 PM | Clear | ___ | ~1000 W/m² |
| 2:00 PM | Clear | ___ | ~900 W/m² |
| 4:00 PM | Clear | ___ | ~500 W/m² |

Plot ADC vs W/m² and fit a curve (linear or polynomial)

### Step 6: Weather-Based Lookup Table

For better accuracy, create a lookup table:

```cpp
float calibrateLDR(int adcValue) {
  // Linear interpolation between known points
  if (adcValue < 500) return 0;
  else if (adcValue < 1000) return map(adcValue, 500, 1000, 0, 200);
  else if (adcValue < 2000) return map(adcValue, 1000, 2000, 200, 500);
  else if (adcValue < 3000) return map(adcValue, 2000, 3000, 500, 800);
  else return map(adcValue, 3000, 4095, 800, 1200);
}
```

---

## 5️⃣ System-Wide Calibration

### Cross-Validation Test

Run system for 1 week and compare with:
1. Weather service solar data
2. Known panel specifications
3. Manual multimeter spot checks

### Expected Performance

| Parameter | Expected Range | Action if Outside |
|-----------|----------------|-------------------|
| Efficiency | 70-95% | Check panel cleanliness |
| Panel Temp | Ambient +5 to +30°C | Verify temp sensor |
| Current | 0-0.6A (10W panel) | Check current sensor |
| Voltage | 15-19V (18V panel) | Check voltage divider |

---

## 📊 Calibration Data Sheet

Fill this out and keep with your project:

```
===============================================
SOLAR PANEL MONITORING SYSTEM
CALIBRATION DATA SHEET
===============================================

Date: _______________
Calibrated By: _______________
Location: _______________

VOLTAGE SENSOR:
  Divider Ratio: _______________
  Correction Factor: _______________
  Max Error: _____% at _____V

CURRENT SENSOR:
  ACS712 Offset: _____V
  Sensitivity: _____mV/A
  Correction Factor: _______________
  Max Error: _____% at _____A

PANEL TEMPERATURE:
  Offset: _____°C
  Scale: _______________
  Max Error: _____°C

AMBIENT TEMPERATURE:
  Offset: _____°C
  Scale: _______________
  Max Error: _____°C

IRRADIANCE SENSOR:
  LDR Min ADC: _______________
  LDR Max ADC: _______________
  Scale Factor: _______________
  Calibration Method: _______________

SOLAR PANEL SPECS:
  Rated Power: _____W
  Rated Voltage: _____V
  Rated Current: _____A
  Panel Area: _____m²
  Measured Efficiency: _____%

ENVIRONMENTAL DATA:
  Location Latitude: _______________
  Location Longitude: _______________
  Average Peak Irradiance: _____W/m²
  Typical Panel Temp: _____°C

NOTES:
_____________________________________________
_____________________________________________
_____________________________________________

Verified By: _______________  Date: _______
```

---

## 🔄 Re-Calibration Schedule

- **Weekly:** Visual inspection, spot checks
- **Monthly:** Zero current check (ACS712)
- **Quarterly:** Full calibration if accuracy <95%
- **Annually:** Complete system recalibration
- **After Changes:** Any hardware modifications

---

## ⚠️ Troubleshooting Calibration Issues

### Issue: Cannot achieve <5% accuracy

**Possible Causes:**
1. Poor quality components (cheap resistors with high tolerance)
2. Noise on ADC lines (add more filtering capacitors)
3. Unstable power supply
4. Temperature effects on components

**Solutions:**
1. Use 1% tolerance resistors for voltage divider
2. Add 100nF caps close to each sensor
3. Use stable lab power supply for calibration
4. Perform calibration at constant temperature

### Issue: Readings drift over time

**Possible Causes:**
1. Temperature effects
2. Component aging
3. Poor connections
4. EMI from solar panel

**Solutions:**
1. Add temperature compensation
2. Use quality components
3. Solder all connections
4. Shield sensor wires

### Issue: LDR calibration inaccurate

**Possible Causes:**
1. Side light reaching LDR
2. LDR non-linearity
3. Cloudy conditions during calibration
4. Dirt/dust on LDR

**Solutions:**
1. Mount LDR in tube (straw)
2. Use multi-point calibration curve
3. Calibrate only on clear days
4. Keep LDR clean

---

## 📱 Mobile Calibration Apps

Useful smartphone apps:
- **Lux Meter** (measure light levels)
- **Thermometer** (ambient temperature)
- **Multimeter apps** (via bluetooth multimeter)
- **Solar Calculator** (estimate irradiance)

---

## 📚 References

1. LM35 Datasheet: https://www.ti.com/lit/ds/symlink/lm35.pdf
2. ACS712 Datasheet: https://www.allegromicro.com/
3. Solar Irradiance Data: https://re.jrc.ec.europa.eu/pvg_tools/en/
4. Egypt Solar Atlas: https://solargis.com/maps-and-gis-data/

---

**Last Updated:** March 2025  
**Version:** 1.0
