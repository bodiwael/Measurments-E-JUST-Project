# Bill of Materials (BOM)

Complete list of components needed for the Solar Panel Monitoring System.

## 📦 Components List

### Core Electronics

| # | Component | Specification | Qty | Unit Price (EGP) | Total (EGP) | Suppliers |
|---|-----------|---------------|-----|------------------|-------------|-----------|
| 1 | ESP32 Development Board | WROOM-32, 38-Pin | 1 | 170 | 170 | Makers Electronics |
| 2 | ACS712 Current Sensor | 30A, Hall Effect | 1 | 50 | 50 | Makers Electronics |
| 3 | LM35 Temperature Sensor | TO-92, 0-100°C | 2 | 15 | 30 | Makers Electronics |
| 4 | LDR Photoresistor | 5mm, 1kΩ-1MΩ | 5 | 3 | 15 | Local Electronics |
| 5 | 16x2 LCD Display | I2C Module | 1 | 70 | 70 | Makers Electronics |
| 6 | MG996R Servo Motor | Metal gear, 11kg·cm | 1-2 | 80 | 80-160 | Makers Electronics |
| 7 | **XL4015 Buck Converter** | **5A, Adjustable (MPPT)** | **1** | **60** | **60** | **Makers Electronics** |

**Subtotal Core:** 475-555 EGP

### Passive Components

| # | Component | Specification | Qty | Unit Price (EGP) | Total (EGP) |
|---|-----------|---------------|-----|------------------|-------------|
| 7 | 100kΩ Resistor | 1/4W, ±5% | 5 | 0.5 | 2.5 |
| 8 | 10kΩ Resistor | 1/4W, ±5% | 15 | 0.5 | 7.5 |
| 9 | 100nF Capacitor | Ceramic, 50V | 10 | 1 | 10 |
| 10 | 10µF Capacitor | Electrolytic, 16V | 5 | 2 | 10 |

**Subtotal Passive:** 30 EGP

### Power Supply

| # | Component | Specification | Qty | Unit Price (EGP) | Total (EGP) |
|---|-----------|---------------|-----|------------------|-------------|
| 11 | TP4056 Charging Module | Li-ion, 1A | 1 | 20 | 20 |
| 12 | 18650 Battery | 3.7V, 2000-3000mAh | 1 | 60 | 60 |
| 13 | 18650 Battery Holder | 1-cell | 1 | 8 | 8 |
| 14 | AMS1117 Voltage Regulator | 3.3V, 800mA | 1 | 5 | 5 |
| 15 | 5V 2A Power Adapter | Micro USB | 1 | 50 | 50 |

**Subtotal Power:** 143 EGP

### Wiring & Assembly

| # | Component | Specification | Qty | Unit Price (EGP) | Total (EGP) |
|---|-----------|---------------|-----|------------------|-------------|
| 16 | Breadboard | 830 points | 1 | 40 | 40 |
| 17 | Jumper Wires M-M | 40pcs, 20cm | 1 | 25 | 25 |
| 18 | Jumper Wires M-F | 40pcs, 20cm | 1 | 25 | 25 |
| 19 | Jumper Wires F-F | 40pcs, 20cm | 1 | 25 | 25 |
| 20 | Dupont Connectors | Assorted | 1 pack | 15 | 15 |
| 21 | Heat Shrink Tubing | Assorted sizes | 1m | 10 | 10 |

**Subtotal Wiring:** 140 EGP

### Enclosure & Mounting

| # | Component | Specification | Qty | Unit Price (EGP) | Total (EGP) |
|---|-----------|---------------|-----|------------------|-------------|
| 22 | Project Box | IP65, Weatherproof | 1 | 80 | 80 |
| 23 | PCB Stripboard | 10x8cm | 1 | 30 | 30 |
| 24 | Servo Bracket | Aluminum, U-shaped | 1-2 | 50 | 50-100 |
| 25 | M3 Bolts | Stainless, 10mm | 20 | 0.5 | 10 |
| 26 | M3 Nuts | Stainless | 20 | 0.3 | 6 |
| 27 | Cable Ties | 100mm, white | 20 | 0.5 | 10 |
| 28 | Silicone Sealant | Weatherproof | 1 tube | 20 | 20 |

**Subtotal Enclosure:** 206-256 EGP

### Solar Panel

| # | Component | Specification | Qty | Unit Price (EGP) | Total (EGP) |
|---|-----------|---------------|-----|------------------|-------------|
| 29 | Solar Panel | 10W, 18V, Monocrystalline | 1 | 300 | 300 |

**Subtotal Solar:** 300 EGP

---

## 💰 Total Cost Summary

### Configuration 1: Basic System (No Tracking, No MPPT)
| Category | Cost (EGP) |
|----------|------------|
| Core Electronics | 335 |
| Passive Components | 30 |
| Power Supply | 143 |
| Wiring & Assembly | 140 |
| Enclosure & Mounting | 176 |
| Solar Panel | 300 |
| **TOTAL** | **1,124 EGP** (~$36 USD) |

### Configuration 2: With MPPT
| Category | Cost (EGP) |
|----------|------------|
| Basic System | 1,124 |
| XL4015 Buck Converter | 60 |
| **TOTAL** | **1,184 EGP** (~$38 USD) |

### Configuration 3: With MPPT + Single-Axis Tracking
| Category | Cost (EGP) |
|----------|------------|
| Basic System + MPPT | 1,184 |
| Additional Servo | 80 |
| Additional LDRs (4) | 12 |
| Servo Bracket | 50 |
| **TOTAL** | **1,326 EGP** (~$43 USD) |

### Configuration 4: Full System (MPPT + Dual-Axis Tracking)
| Category | Cost (EGP) |
|----------|------------|
| Basic System + MPPT | 1,184 |
| Additional Servos (2) | 160 |
| Additional LDRs (4) | 12 |
| Servo Brackets (2) | 100 |
| **TOTAL** | **1,456 EGP** (~$47 USD) |

---

## 🛒 Recommended Suppliers

### Egypt - Alexandria

#### 1. Makers Electronics ⭐ **PRIMARY**
- **Website:** https://makerselectronics.com
- **Address:** 158 Elhourya St., Elibrahymia, Alexandria
- **Phone:** +20 2 48813824
- **WhatsApp:** +20 1211981188
- **Email:** info@makerselectronics.com
- **Shipping:** Free over 1000 EGP
- **Stock:** Most components available

#### 2. Electra Store
- **Website:** https://electra.store
- **Address:** 47 Ahmed Kamha St., Camp Caesar, Alexandria
- **Phone:** +20 3 5912212
- **Payment:** Cash on Delivery available

#### 3. Local Electronics Markets
- **Rod El Farag Market** (Cairo) - Best prices
- **Attaba Electronics Shops** (Downtown Cairo)
- **Mansheya Electronics Market** (Alexandria)

### Online Stores (Egypt)
- Egypt Laptop: egyptlaptop.com
- Souq.com / Noon.com
- Jumia Egypt

---

## 📋 Optional Upgrades

### Advanced Features

| Component | Purpose | Price (EGP) |
|-----------|---------|-------------|
| SD Card Module + 16GB Card | Data logging | 60 |
| DS3231 RTC Module | Accurate timestamps | 45 |
| 5V Relay Module | Safety disconnect | 20 |
| Buzzer Module | Audio alerts | 15 |
| RGB LED | Status indicators | 8 |
| BME280 Sensor | Humidity monitoring | 90 |
| 0.96" OLED Display | Better display | 100 |
| GPS Module | Location tracking | 120 |

---

## 🔧 Tools Required

### Essential Tools

| Tool | Purpose | Approximate Cost (EGP) |
|------|---------|------------------------|
| Soldering Iron | Circuit assembly | 150-400 |
| Solder Wire | Connections | 30 |
| Wire Strippers | Cable preparation | 40 |
| Multimeter | Testing & debugging | 100-500 |
| Screwdriver Set | Assembly | 80 |
| Hot Glue Gun | Mounting | 50 |
| Drill | Enclosure holes | 200-800 |

### Optional Tools

| Tool | Purpose | Approximate Cost (EGP) |
|------|---------|------------------------|
| Oscilloscope | Signal debugging | 2000+ |
| Power Supply | Testing | 500-2000 |
| Heat Gun | Heat shrink | 150 |
| Crimp Tool | Connector assembly | 100 |
| Cable Tester | Wire checking | 80 |

---

## 📦 Packaging & Shipping

### If Ordering All Components from Makers Electronics

**Estimated Package Details:**
- Weight: ~1.5 kg
- Volume: 30cm × 20cm × 10cm
- Shipping within Egypt: 3-7 days
- Tracking: Available
- Insurance: Recommended for total value >500 EGP

**Tips:**
1. Order everything at once to save on shipping
2. Check stock availability via WhatsApp before ordering
3. Request component testing before shipping (if possible)
4. Keep original packaging for potential returns

---

## ✅ Pre-Purchase Checklist

Before ordering:
- [ ] Finalize system configuration (basic/single-axis/dual-axis)
- [ ] Verify all component specifications
- [ ] Check supplier stock availability
- [ ] Calculate total budget including shipping
- [ ] Prepare payment method (cash/card/online)
- [ ] Confirm delivery address
- [ ] Have WhatsApp number ready for supplier contact
- [ ] Download datasheets for components
- [ ] Plan assembly workspace

---

## 📊 Cost Comparison

### Egypt vs International

| Item | Egypt (EGP) | AliExpress (USD) | Amazon US (USD) |
|------|-------------|------------------|-----------------|
| ESP32 | 170 (~$5.5) | $3-5 | $8-12 |
| ACS712 | 50 (~$1.6) | $1-2 | $5-8 |
| LM35 | 15 (~$0.5) | $0.50-1 | $2-4 |
| MG996R Servo | 80 (~$2.6) | $2-4 | $8-15 |
| LCD 16x2 I2C | 70 (~$2.3) | $2-3 | $6-10 |

**Notes:**
- Egypt prices include local availability & support
- International shipping: 2-4 weeks + customs
- Egypt total system: ~$36-45 USD
- International total: ~$30-40 USD + $10-20 shipping

**Conclusion:** Local purchase in Egypt is competitive, faster, and includes support!

---

## 💡 Money-Saving Tips

1. **Buy Kits:** Some suppliers offer Arduino starter kits with resistors/capacitors
2. **Reuse Components:** Old electronics may have usable resistors, capacitors
3. **Group Order:** Split shipping costs with classmates
4. **Breadboard First:** Perfect the circuit before making PCB
5. **Generic Brands:** Chinese components work well for projects
6. **University Lab:** Check if university has component library
7. **Wait for Sales:** Makers Electronics occasionally has discounts
8. **Borrow Tools:** Share tools with team members
9. **Free Software:** Use Arduino IDE, Fritzing, KiCad (all free)
10. **Incremental Build:** Start basic, add tracking later

---

## 📝 Notes

1. All prices are estimates as of March 2025
2. Prices may vary based on supplier and availability
3. EGP to USD conversion: 1 USD ≈ 31 EGP
4. Add 10-15% buffer for unexpected costs
5. Some components may be sold in packs (e.g., resistors)
6. Quality varies - prefer known brands for critical components
7. Keep receipts for academic expense reporting
8. Buy extra connectors/wires - very useful during assembly

---

**Last Updated:** March 2025  
**Currency:** Egyptian Pounds (EGP)  
**Version:** 1.0
