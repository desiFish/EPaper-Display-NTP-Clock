<div align="center">
  <h1>🌟 Minimalistic E-Paper NTP-RTC Clock 🕒</h1>
  <p>
    <img src="https://img.shields.io/badge/ESP32-C3-blue?style=flat-square&logo=espressif"/>
    <img src="https://img.shields.io/badge/Battery-6000mAh-green?style=flat-square"/>
    <img src="https://img.shields.io/badge/Display-3.52%22-orange?style=flat-square"/>
    <img src="https://img.shields.io/badge/Eco-Friendly-brightgreen?style=flat-square"/>
  </p>
  <p>
    <img src="https://img.shields.io/badge/status-active-success.svg" alt="Status">
    <a href="/LICENSE"><img src="https://img.shields.io/github/license/desiFish/ESP32-ePaper-Display-NTP-Clock" alt="License"></a>
    <a href="https://github.com/desiFish/ESP32-ePaper-Display-NTP-Clock/issues"><img src="https://img.shields.io/github/issues/desiFish/ESP32-ePaper-Display-NTP-Clock.svg" alt="GitHub Issues"></a>
    <a href="https://github.com/desiFish/ESP32-ePaper-Display-NTP-Clock/releases"><img src="https://img.shields.io/github/v/release/desiFish/ESP32-ePaper-Display-NTP-Clock" alt="Release"></a>
  </p>
</div>

> ⚠️ **IMPORTANT**: Please read the [Notes & Warnings] section carefully before proceeding with this project. It contains crucial safety information about battery configuration, charging, and sensor placement.

<details>
<summary>📊 System Specifications</summary>

```ini
Microcontroller: Xiao ESP32 C3
Display: 3.52 inches BiColor E-paper (360x240)
Battery: LiFePO4 6000mAh 3.2V
Operating Voltage: 3.2V
Sleep Duration: 60 seconds
I2C Speed: 400kHz
Battery Thresholds:
  - High: 3.4V
  - Low: 2.9V
NTP Server: asia.pool.ntp.org
Time Offset: 19800 (India)
```
</details>

## 🛠️ Hardware Components
<table>
<tr>
  <th>Component</th>
  <th>Specifications</th>
  <th>Details</th>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/esp32c3.jpg" width="100"><br>Xiao ESP32 C3</td>
  <td>
    • MCU: ESP32-C3 RISC-V<br>
    • Clock: 160MHz<br>
    • RAM: 400KB SRAM<br>
    • Flash: 4MB<br>
    • WiFi: 2.4GHz<br>
    • Bluetooth: BLE 5.0<br>
    • GPIO: 11 pins<br>
    • Size: 21x17.5mm<br>
    • Deep Sleep: ~45µA
  </td>
  <td>Ultra-compact IoT controller with low power consumption and built-in wireless capabilities</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/epd.jpg" width="100"><br>Waveshare 3.52" E-Paper</td>
  <td>
    • Resolution: 360x240<br>
    • Colors: BiColor<br>
    • Refresh: 1.5 secs
  </td>
  <td>Display unit with low power consumption</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/bh1750.jpg" width="100"><br>BH1750</td>
  <td>
    • I2C Address: 0x23<br>
    • Mode: ONE_TIME_HIGH_RES<br>
    • Precision: 1-65535 lux
  </td>
  <td>Ambient light sensor for night mode</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/ds3231.jpg" width="100"><br>DS3231</td>
  <td>
    • Accuracy: ±2ppm<br>
    • Temperature comp.<br>
    • Battery backup
  </td>
  <td>Real-time clock for timekeeping</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/battery.jpg" width="100"><br>IFR32650 LiFePO4</td>
  <td>
    • Model: IFR32650<br>
    • Capacity: 6000mAh<br>
    • Voltage: 3.2V nominal<br>
    • Chemistry: LiFePO4<br>
    • Cycle Life: >2000 cycles<br>
    • Size: 32x65mm<br>
    • Max Discharge: 2C<br>
    • Temperature Range: -20°C to 60°C<br>
    • Self-discharge: 3% per month
  </td>
  <td>High-capacity, safe chemistry battery with long cycle life and stable voltage output</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/tp5000.jpg" width="100"><br>TP5000</td>
  <td>
    • Input: 5V USB<br>
    • Output: 3.2V<br>
    • Current: Up to 2A
  </td>
  <td>Battery charging module</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/usbc.jpg" width="100"><br>USB-C Breakout</td>
  <td>
    • USB 2.0 Compatible<br>
    • 5V Input<br>
    • Data lines available
  </td>
  <td>Power input and debugging interface</td>
</tr>
</table>

## 💡 Key Features
<div align="center">
  <table>
    <tr>
      <td align="center">⚡<br>Power Efficient</td>
      <td align="center">🌙<br>Night Mode</td>
      <td align="center">🔄<br>Auto Update</td>
      <td align="center">📊<br>Battery Monitor</td>
    </tr>
    <tr>
      <td>60s Deep Sleep</td>
      <td>Light Sensor Based</td>
      <td>Daily NTP Sync</td>
      <td>Voltage Tracking</td>
    </tr>
  </table>
</div>

## 📈 System Architecture
```mermaid
graph TD
    A[Wake Up] --> B{Check Light}
    B -->|Dark| C[Sleep Mode]
    B -->|Light| D[Update Display]
    D --> E{Battery OK?}
    E -->|Yes| F[Update Time]
    F --> G[Update Display]
    G --> H[Deep Sleep]
```

## 🚀 Quick Start
<details>
<summary>Prerequisites</summary>

```yaml
Required Libraries:
  - Check header section inside .ino file
  - OEM Display Libraries (Included)
```
</details>

<details>
<summary>Installation Steps</summary>

1. Clone repository
2. Configure WiFi:
```cpp
const char *ssid = "Your_SSID";
const char *password = "Your_PASSWORD";
```
3. Update pins in `src/epdif.h`
4. Upload code
</details>

## 🔋 Battery Performance
<div align="center">
<table>
  <tr>
    <th colspan="4">📊 Battery Life Analysis - Test #1</th>
  </tr>
  <tr>
    <td><strong>Start Date</strong></td>
    <td><strong>End Date</strong></td>
    <td><strong>Runtime</strong></td>
    <td><strong>Status</strong></td>
  </tr>
  <tr>
    <td>June 6, 2024</td>
    <td>November 1, 2024</td>
    <td>4 months 27 days</td>
    <td>✅ Completed</td>
  </tr>
  <tr>
    <td colspan="4">
      <strong>Performance Metrics</strong><br>
      ⚡ Average Current Draw: ~60µA<br>
      🔋 Initial Voltage: 3.6V<br>
      📉 Final Voltage: ~2.84V<br>
      🌡️ Temperature Range: 17-38°C
    </td>
  </tr>
</table>

<br>

<table>
  <tr>
    <th colspan="4">📊 Battery Life Analysis - Test #2</th>
  </tr>
  <tr>
    <td><strong>Start Date</strong></td>
    <td><strong>End Date</strong></td>
    <td><strong>Runtime</strong></td>
    <td><strong>Status</strong></td>
  </tr>
  <tr>
    <td>November 1, 2024</td>
    <td>Coming Soon</td>
    <td>In Progress</td>
    <td>⏳ Running</td>
  </tr>
  <tr>
    <td colspan="4">
      <strong>Performance Metrics</strong><br>
      ⚡ Average Current Draw: Measuring...<br>
      🔋 Initial Voltage: 3.6V<br>
      📉 Final Voltage: TBD<br>
      🌡️ Temperature Range: Monitoring...
    </td>
  </tr>
</table>
</div>

## 📷 Gallery
<div align="center">
<table border="0" cellspacing="0" cellpadding="10">
  <tr>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/epdClock.jpg" width="200"><br>
      <sub>Front 1</sub>
    </td>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/x1.jpg" width="200"><br>
      <sub>Side 1</sub>
    </td>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/x2.jpg" width="200"><br>
      <sub>Back</sub>
    </td>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/x3.jpg" width="200"><br>
      <sub>Front 2</sub>
    </td>
  </tr>
  <tr>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/x4.jpg" width="200"><br>
      <sub>Inside 1</sub>
    </td>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/x5.jpg" width="200"><br>
      <sub>Inside 2</sub>
    </td>
    <td align="center">
      <img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/images/x6.jpg" width="200"><br>
      <sub>Inside 3</sub>
    </td>
    <td align="center">
      <img src="" width="200"><br>
      <sub>Coming Soon</sub>
    </td>
  </tr>
</table>
</div>

## 🛠️ Development
<details>
<summary>Build Requirements</summary>

- 💻 Arduino IDE 2.3.x or newer (tested)
- 📦 ESP32 Board Support Package
- 🔧 USB-C cable for programming
- 📚 Required libraries (see Prerequisites)
</details>

<details>
<summary>Debug Tips</summary>

- 🔍 Serial Monitor: 115200 baud
- 📊 Test points available for voltage monitoring (just lift the cover)
</details>

## 📱 Connectivity
- 📡 WiFi6 2.4GHz
- 🕒 NTP synchronization
- 🔄 Daily updates
- 💤 Auto sleep when inactive

## 🔬 Technical Details
- 📊 ADC Resolution: 12-bit
- 🔌 I2C Speed: 400kHz
- 🔋 Power Modes:
  - Active
  - Deep Sleep

## ⚠️ Important Notes & Warnings

### Battery Configuration Safety

### Power Management Considerations
- ⚡ **Current Draw**: This project's extremely low current consumption (well below rated cell capacity) makes the cell configuration safer
- 🛡️ **Protection**: Always use proper BMS protection for your cells
- 📝 **BMS Setup**: Due to limited 1S LiFePO4 BMS availability:
  - Using 4.2V BMS for 2.5V cutoff protection (Low Discharge Cutoff)
  - TP5000 connected directly to the cell for proper 3.6V charging
  - This is a temporary solution until better 1S LFP BMS options become available

### USB Charging Safety
- ⚡ **USB Connection Warning**: Disconnect battery when connecting USB to ESP32C3
  - XIAO modules are designed for LiPo/Li-ion batteries (4.2V charging)
  - Not compatible with LFP battery charging (3.6V required)
  - Dedicated USB-C module recommended
  - Connect USB-C module output to TP5000 input
  - Detailed schematics will be provided later
  - Simple setup despite complex explanation

### Voltage Compatibility
- ✅ **Operating Range**: 
  - All components work perfectly with LFP's lower voltage
  - Tested operational down to 2.8V
  - No impact on battery life or performance
  - LFP cells maintain stable 3.2V for extended periods
  - 6+ months of testing
  - Components designed for wide voltage range operation

### Enclosure Design
- 💨 **Ventilation Requirements**: 
  - Proper air vents are crucial
  - Ensures accurate sensor readings
  - Prevents heat buildup

> ⚠️ **Please consider these points carefully before replicating this setup. Battery safety is crucial!**
---

## 📜 License
<details>
<summary>GNU General Public License v3.0</summary>

This project is licensed under the GNU GPL v3.0 - see below for details:

- ✅ Commercial use
- ✅ Modification
- ✅ Distribution
- ✅ Patent use
- ✅ Private use

#### Limitations
- ⚠️ Liability
- ⚠️ Warranty
- ℹ️ License and copyright notice required
- ℹ️ State changes
- ℹ️ Disclose source
- ℹ️ Same license

[Read full license](/LICENSE)
</details>

## 🤝 Contributing
We welcome contributions! Here's how you can help:

1. 🔍 Fork the repository
2. 🌿 Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. 💾 Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. 🚀 Push to the branch (`git push origin feature/AmazingFeature`)
5. 📝 Open a Pull Request

### Pull Request Guidelines
- ✅ Clear description of changes
- ✅ Test your changes thoroughly
- ✅ Update documentation if needed
- ✅ Follow existing code style
- ✅ Add comments where necessary
---
<div align="center">
<p>Made with ❤️ and ♻️ materials</p>
</div>