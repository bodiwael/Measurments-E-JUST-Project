# ThingSpeak Setup Guide

Complete step-by-step guide to configure ThingSpeak for the Solar Panel Monitoring System.

## 📋 Table of Contents
1. [Create Account](#create-account)
2. [Create Channel](#create-channel)
3. [Get API Keys](#get-api-keys)
4. [Configure Visualizations](#configure-visualizations)
5. [MATLAB Analytics](#matlab-analytics-optional)
6. [Mobile Access](#mobile-access)
7. [Troubleshooting](#troubleshooting)

---

## 1. Create Account

### Step 1.1: Sign Up
1. Go to https://thingspeak.com
2. Click **"Sign Up"** (top right)
3. Enter your details:
   - Email address
   - Create password
   - Accept terms
4. Click **"Create MathWorks Account"**
5. Verify your email (check inbox/spam)

### Step 1.2: Activate Account
1. Click verification link in email
2. Complete profile (optional)
3. You're ready!

**Free Tier Limits:**
- 3 million messages per year
- Update every 15 seconds minimum
- Unlimited channels
- 4 fields per update
- MATLAB analytics included

---

## 2. Create Channel

### Step 2.1: New Channel
1. Login to ThingSpeak
2. Click **"Channels"** → **"My Channels"**
3. Click **"New Channel"**

### Step 2.2: Channel Settings
Fill in the following:

**Basic Information:**
```
Name: Solar Panel Monitor
Description: Real-time solar panel performance monitoring system
```

**Fields Configuration:**
```
Field 1: Voltage (V)
Field 2: Current (A)
Field 3: Power (W)
Field 4: Irradiance (W/m²)
Field 5: Panel Temperature (°C)
Field 6: Expected Power (W)
Field 7: Efficiency (%)
Field 8: Ambient Temperature (°C)
```

**Optional Settings:**
- ☑️ Show Channel Location (add your city)
- ☑️ Make Public (if you want to share)
- Metadata: Add tags like "solar", "renewable energy", "IoT"

### Step 2.3: Save Channel
1. Click **"Save Channel"** (bottom)
2. Your channel is created! Note the **Channel ID** at the top

---

## 3. Get API Keys

### Step 3.1: Write API Key
1. Go to your channel page
2. Click **"API Keys"** tab
3. Find **"Write API Key"** section
4. Copy the key (16 characters) → **This goes in config.h**

Example: `ABCDEFGH12345678`

### Step 3.2: Read API Key
- Copy **"Read API Key"** (for mobile apps, optional)

### Step 3.3: MQTT Credentials
For MQTT connection (used by ESP32):

1. Click your **profile name** (top right)
2. Go to **"My Profile"**
3. Scroll to **"MQTT"** section
4. Find:
   - **Client ID** → Copy this
   - **Username** → Copy this
   - **Password** (MQTT API Key) → Copy this

### Step 3.4: Update config.h
Open `main/config.h` and fill in:

```cpp
#define THINGSPEAK_CHANNEL_ID "1234567"              // Your Channel ID
#define THINGSPEAK_WRITE_API_KEY "ABCDEFGH12345678"  // Write API Key
#define THINGSPEAK_MQTT_USERNAME "YourMQTTUsername"  // MQTT Username
#define THINGSPEAK_MQTT_PASSWORD "YourMQTTClientID"  // MQTT API Key
```

---

## 4. Configure Visualizations

### Step 4.1: Private View
1. Go to your channel
2. Click **"Private View"** tab
3. Click **"+ Add Widgets"**

### Step 4.2: Add Widgets

#### Widget 1: Gauge (Efficiency)
```
Type: Gauge
Field: Field 7 (Efficiency)
Min Value: 0
Max Value: 100
Label: Panel Efficiency (%)
Color Ranges:
  0-50: Red (Poor)
  50-70: Yellow (Fair)
  70-85: Blue (Good)
  85-100: Green (Excellent)
```

#### Widget 2: Numeric Display (Current Power)
```
Type: Numeric Display
Field: Field 3 (Power)
Suffix: W
Decimals: 2
Label: Current Power Output
```

#### Widget 3: Line Chart (Power Comparison)
```
Type: Line Chart
Y-Axis:
  - Field 3 (Power) - Blue line
  - Field 6 (Expected Power) - Red dashed line
X-Axis: Time
Results: 100 (last 100 readings)
Label: Actual vs Expected Power
```

#### Widget 4: Line Chart (Voltage Over Time)
```
Type: Line Chart
Field: Field 1 (Voltage)
Results: 100
Label: Panel Voltage (V)
Color: Green
```

#### Widget 5: Line Chart (Temperature)
```
Type: Line Chart
Y-Axis:
  - Field 5 (Panel Temp) - Orange
  - Field 8 (Ambient Temp) - Blue
Results: 100
Label: Temperature Monitoring
```

#### Widget 6: Numeric Display (Irradiance)
```
Type: Numeric Display
Field: Field 4 (Irradiance)
Suffix: W/m²
Label: Solar Irradiance
```

### Step 4.3: Arrange Dashboard
1. Drag widgets to organize layout
2. Resize as needed
3. Save arrangement

---

## 5. MATLAB Analytics (Optional)

ThingSpeak includes free MATLAB for data analysis.

### Example 1: Daily Energy Calculation
1. Go to **"Apps"** → **"MATLAB Analysis"**
2. Click **"New"**
3. Paste this code:

```matlab
% Calculate daily energy production
readChannelID = YOUR_CHANNEL_ID;
readAPIKey = 'YOUR_READ_API_KEY';

% Read power data from last 24 hours
powerData = thingSpeakRead(readChannelID, 'Fields', 3, ...
                          'NumDays', 1, 'ReadKey', readAPIKey);

% Calculate energy (assuming 5-minute intervals)
% Energy (Wh) = Sum of Power × Time interval
timeInterval = 5/60;  % 5 minutes in hours
energy_wh = sum(powerData) * timeInterval;
energy_kwh = energy_wh / 1000;

% Display result
display(['Daily Energy Production: ', num2str(energy_kwh), ' kWh']);

% Write to another channel field (optional)
% thingSpeakWrite(channelID, energy_kwh, 'WriteKey', writeAPIKey);
```

### Example 2: Efficiency Trend Analysis
```matlab
% Analyze efficiency trend
efficiencyData = thingSpeakRead(readChannelID, 'Fields', 7, ...
                                'NumDays', 7, 'ReadKey', readAPIKey);

% Calculate statistics
avgEfficiency = mean(efficiencyData);
maxEfficiency = max(efficiencyData);
minEfficiency = min(efficiencyData);

% Display
display(['Average Efficiency: ', num2str(avgEfficiency), '%']);
display(['Peak Efficiency: ', num2str(maxEfficiency), '%']);
display(['Lowest Efficiency: ', num2str(minEfficiency), '%']);

% Plot trend
plot(efficiencyData);
title('7-Day Efficiency Trend');
xlabel('Reading Number');
ylabel('Efficiency (%)');
grid on;
```

### Schedule MATLAB Analytics
1. Create MATLAB script
2. Click **"Actions"** → **"Time Control"**
3. Set schedule (e.g., run daily at midnight)
4. Save

---

## 6. Mobile Access

### Option 1: ThingSpeak Mobile App
**iOS:**
1. Download "ThingView" from App Store
2. Enter your Channel ID
3. Add Read API Key
4. Done!

**Android:**
1. Download "ThingSpeak" from Play Store
2. Login with your account
3. View your channels

### Option 2: Mobile Web
- Simply visit: `https://thingspeak.com/channels/YOUR_CHANNEL_ID`
- Works on any mobile browser

### Option 3: Custom Mobile App (Blynk)
See main README for Blynk integration instructions.

---

## 7. Troubleshooting

### Issue: "Unauthorized" Error
**Cause:** Wrong API keys

**Solution:**
1. Double-check Write API Key in config.h
2. Ensure no extra spaces when copying
3. Verify Channel ID is correct
4. Check MQTT credentials

### Issue: Data Not Updating
**Cause:** Update rate too fast or network issue

**Solution:**
1. Check ESP32 Serial Monitor for errors
2. Verify `UPLOAD_INTERVAL` is ≥15000 (15 seconds)
3. Test internet connection
4. Check ThingSpeak status: https://status.thingspeak.com

### Issue: Fields Empty/Wrong Values
**Cause:** Field number mismatch

**Solution:**
1. Verify field numbers in code match channel setup:
   ```cpp
   field1 = Voltage
   field2 = Current
   field3 = Power
   // etc.
   ```
2. Check Serial Monitor for actual values being sent

### Issue: MQTT Connection Failed
**Cause:** Firewall or incorrect MQTT settings

**Solution:**
1. Verify MQTT broker: `mqtt3.thingspeak.com`
2. Port should be `1883`
3. Check network allows MQTT (port 1883)
4. Use HTTP API as alternative:
   ```cpp
   // In sendToThingSpeak(), replace MQTT with HTTP:
   String url = "https://api.thingspeak.com/update?api_key=" + 
                String(THINGSPEAK_WRITE_API_KEY) + 
                "&field1=" + String(voltage);
   // ... add all fields
   ```

### Issue: Channel Shows Old Data
**Cause:** ESP32 not uploading

**Solution:**
1. Check ESP32 power
2. Verify WiFi connection (check Serial Monitor)
3. Restart ESP32
4. Check MQTT connection status

### Test Data Upload
Test with browser:
```
https://api.thingspeak.com/update?api_key=YOUR_WRITE_KEY&field1=17.5&field2=0.5&field3=8.75
```

Should return entry number if successful.

---

## 🎯 Quick Reference

### Important URLs
- ThingSpeak Homepage: https://thingspeak.com
- Documentation: https://www.mathworks.com/help/thingspeak
- API Reference: https://www.mathworks.com/help/thingspeak/rest-api.html
- MATLAB Tutorials: https://www.mathworks.com/help/thingspeak/matlab-analysis.html
- Status Page: https://status.thingspeak.com

### Free Tier Limits
- **Messages/Year:** 3,000,000
- **Message Rate:** 1 message every 15 seconds
- **Channels:** Unlimited
- **Data Retention:** Forever
- **MATLAB:** Included
- **Charts:** Unlimited

### MQTT Settings
```
Broker: mqtt3.thingspeak.com
Port: 1883
Protocol: MQTT 3.1.1
QoS: 0 or 1
Topic: channels/YOUR_CHANNEL_ID/publish
Payload: field1=value&field2=value...
```

### HTTP API (Alternative)
```
Method: GET or POST
URL: https://api.thingspeak.com/update
Parameters:
  api_key=YOUR_WRITE_KEY
  field1=value
  field2=value
  ...
```

---

## 📧 Support

**ThingSpeak Support:**
- Community: https://www.mathworks.com/matlabcentral/answers
- Documentation: https://www.mathworks.com/help/thingspeak
- Contact: support@thingspeak.com

**Project Support:**
- GitHub Issues: [Your repository]/issues
- Team Email: [Your team email]

---

**Last Updated:** March 2025  
**Version:** 1.0
