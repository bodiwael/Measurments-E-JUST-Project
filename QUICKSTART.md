# ⚡ Quick Start Guide

Get your Solar Panel Monitoring System up and running in 30 minutes!

## 📦 What You Need

✅ All components from [BOM.md](BOM.md)  
✅ Computer with Arduino IDE installed  
✅ USB cable for ESP32  
✅ WiFi network (2.4GHz)  
✅ ThingSpeak account (free)

---

## 🚀 5-Step Quick Start

### Step 1: ThingSpeak Setup (5 minutes)

1. Go to https://thingspeak.com and sign up
2. Create new channel with 8 fields:
   - Field 1: Voltage
   - Field 2: Current
   - Field 3: Power
   - Field 4: Irradiance
   - Field 5: Panel Temperature
   - Field 6: Expected Power
   - Field 7: Efficiency
   - Field 8: Ambient Temperature
3. Copy your **Channel ID** and **Write API Key**

**Detailed guide:** [docs/THINGSPEAK_SETUP.md](docs/THINGSPEAK_SETUP.md)

---

### Step 2: Arduino IDE Setup (10 minutes)

1. **Install Arduino IDE**  
   Download from: https://www.arduino.cc/en/software

2. **Add ESP32 Board**  
   - File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Board Manager
   - Search "ESP32" and install "esp32 by Espressif Systems"

3. **Install Required Libraries**  
   Tools → Manage Libraries, search and install:
   - `PubSubClient` (by Nick O'Leary)
   - `LiquidCrystal_I2C` (by Frank de Brabander)
   - `ESP32Servo` (by Kevin Harrington)

---

### Step 3: Configure Code (5 minutes)

1. Open `main/config.h`

2. Fill in your WiFi credentials:
   ```cpp
   #define WIFI_SSID "YourWiFiName"
   #define WIFI_PASSWORD "YourPassword"
   ```

3. Fill in ThingSpeak credentials:
   ```cpp
   #define THINGSPEAK_CHANNEL_ID "1234567"
   #define THINGSPEAK_WRITE_API_KEY "YOUR16CHARKEY"
   ```

4. Adjust solar panel specs (if different):
   ```cpp
   #define PANEL_RATED_POWER 10.0
   #define PANEL_AREA 0.072
   ```

5. Save the file!

---

### Step 4: Hardware Assembly (10 minutes)

**Basic Connections:**

```
ESP32 Pin Connections:
- GPIO 34 → Voltage Divider (100kΩ + 10kΩ)
- GPIO 35 → ACS712 OUT
- GPIO 32 → LDR circuit
- GPIO 33 → LM35 Panel Temp
- GPIO 25 → LM35 Ambient Temp
- GPIO 21 → LCD SDA
- GPIO 22 → LCD SCL
- VIN → 5V Power
- GND → Common Ground
```

**Quick Breadboard Setup:**
1. Insert ESP32 into breadboard
2. Connect power rails (5V, GND)
3. Build voltage divider (100kΩ + 10kΩ to GPIO34)
4. Connect ACS712 module to GPIO35
5. Connect LDR with 10kΩ pulldown to GPIO32
6. Connect 2× LM35 with voltage dividers to GPIO33 & GPIO25
7. Connect LCD I2C to GPIO21 (SDA) and GPIO22 (SCL)

**Detailed wiring:** [docs/CIRCUIT_SCHEMATIC.md](docs/CIRCUIT_SCHEMATIC.md)

---

### Step 5: Upload & Test (5 minutes)

1. **Connect ESP32 to computer via USB**

2. **Select Board and Port:**
   - Tools → Board → ESP32 Arduino → ESP32 Dev Module
   - Tools → Port → (Select your ESP32 port)

3. **Open main sketch:**
   - File → Open → `main/main.ino`

4. **Upload:**
   - Click Upload button (→) or Ctrl+U
   - Wait for "Hard resetting via RTS pin..." message

5. **Open Serial Monitor:**
   - Tools → Serial Monitor
   - Set baud rate to **115200**

6. **Verify Output:**
   ```
   ========================================
   Solar Panel Monitoring System
   ========================================
   
   Initializing LCD...
   Connecting to WiFi: YourWiFiName
   ....
   ✓ WiFi Connected!
   IP Address: 192.168.1.xxx
   
   System Ready!
   
   ========== SOLAR MONITOR ==========
   Voltage: 17.45 V
   Current: 0.523 A
   Power: 9.12 W
   -----------------------------------
   ```

7. **Check ThingSpeak:**
   - Go to your ThingSpeak channel
   - Data should appear within 15-30 seconds

---

## ✅ Quick Verification Checklist

After first power-on:

- [ ] ESP32 blue LED blinks (power)
- [ ] LCD backlight turns on
- [ ] LCD shows "Solar Monitor" then connects to WiFi
- [ ] Serial Monitor shows sensor readings
- [ ] WiFi connects successfully
- [ ] ThingSpeak receives data (check channel)
- [ ] LCD cycles through 3 screens
- [ ] All sensor readings look reasonable

---

## 🐛 Quick Troubleshooting

### WiFi Won't Connect
```
✗ Check SSID and password in config.h
✗ Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
✗ Move closer to router
✗ Try phone hotspot for testing
```

### LCD Not Working
```
✗ Check I2C address (run examples/i2c_scanner)
✗ Try changing 0x27 to 0x3F in config.h
✗ Verify SDA→GPIO21, SCL→GPIO22
✗ Check 5V power to LCD
```

### No Sensor Readings
```
✗ Run examples/sensor_test to check each sensor
✗ Verify all connections
✗ Check common ground
✗ Measure voltages with multimeter
```

### ThingSpeak Not Updating
```
✗ Verify API keys are correct (no spaces)
✗ Check Serial Monitor for MQTT errors
✗ Test manually: open in browser:
   https://api.thingspeak.com/update?api_key=YOURKEY&field1=100
```

**Full troubleshooting:** [README.md#troubleshooting](README.md#troubleshooting)

---

## 📚 Next Steps

Once basic system works:

1. **Calibrate Sensors**  
   Follow [docs/CALIBRATION.md](docs/CALIBRATION.md) for accuracy

2. **Add Solar Tracking** (Optional)  
   Connect servos and enable in config.h

3. **Create Dashboard**  
   Add widgets in ThingSpeak Private View

4. **Field Testing**  
   Mount outdoors, test for 1 week

5. **Weatherproof Enclosure**  
   Seal electronics in IP65 box

6. **Data Analysis**  
   Use ThingSpeak MATLAB for insights

7. **Mobile App**  
   Set up Blynk for smartphone control

---

## 🎓 Learning Resources

### Video Tutorials (Recommended)
- ESP32 Setup: https://www.youtube.com/watch?v=xPlN_Tk3VLQ
- ThingSpeak Guide: https://www.youtube.com/watch?v=x4frZNLm7Bc
- Solar Panel Basics: https://www.youtube.com/watch?v=Yxt72aDjFgY

### Documentation
- [Complete README](README.md) - Full system documentation
- [Circuit Diagrams](docs/CIRCUIT_SCHEMATIC.md) - Detailed wiring
- [ThingSpeak Setup](docs/THINGSPEAK_SETUP.md) - Cloud configuration
- [Bill of Materials](docs/BOM.md) - Component list with links

### Arduino References
- Arduino Language: https://www.arduino.cc/reference/en/
- ESP32 Docs: https://docs.espressif.com/projects/arduino-esp32/
- ThingSpeak API: https://www.mathworks.com/help/thingspeak/

---

## 💡 Pro Tips

1. **Test sensors individually** before final assembly
2. **Use breadboard first** to perfect circuit
3. **Label all wires** during assembly
4. **Take photos** of working connections
5. **Keep backup** of working config.h
6. **Start simple** - add features incrementally
7. **Document issues** you encounter (help teammates!)
8. **Calibrate properly** - accuracy matters for academics
9. **Clean panel regularly** for consistent results
10. **Monitor for 48 hours** before final presentation

---

## 📞 Help & Support

**Stuck? Try these resources:**

1. **Check Serial Monitor** - Most issues show error messages
2. **Run test sketches** - examples/sensor_test/, examples/i2c_scanner/
3. **Search this repo** - Use GitHub search for error messages
4. **Arduino Forums** - https://forum.arduino.cc/
5. **ESP32 Community** - https://www.esp32.com/
6. **ThingSpeak Support** - https://www.mathworks.com/matlabcentral/

**Team Contact:**  
[Add your team members' contact info here]

---

## ⏱️ Timeline Summary

| Task | Time | Cumulative |
|------|------|------------|
| ThingSpeak setup | 5 min | 5 min |
| Arduino IDE setup | 10 min | 15 min |
| Code configuration | 5 min | 20 min |
| Hardware assembly | 10 min | 30 min |
| Upload & test | 5 min | 35 min |
| **Total to working system** | **35 min** | ✅ |

Add more time for:
- Calibration: +2 hours
- Solar tracking: +1 hour
- Enclosure assembly: +1 hour
- Field testing: 1 week
- Documentation: Ongoing

---

## 🎯 Success Criteria

Your system is ready when:

✅ LCD displays rotating data screens  
✅ Serial Monitor shows sensor readings  
✅ WiFi connects automatically  
✅ ThingSpeak updates every 15 seconds  
✅ All sensors read reasonable values  
✅ Efficiency calculation makes sense (70-95%)  
✅ System runs for 24+ hours without issues

---

**You're all set! Happy monitoring! ☀️**

---

**Last Updated:** March 2025  
**Estimated Time:** 30-60 minutes for basic system  
**Difficulty:** Beginner-Intermediate
