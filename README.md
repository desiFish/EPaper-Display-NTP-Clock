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

<details>
<summary>📊 System Specifications</summary>

```ini
Microcontroller: Xiao ESP32 C3
Display: 3.52" BiColor E-paper (360x240)
Battery: LiFePO4 6000mAh 3.2V
Operating Voltage: 3.2V
Sleep Duration: 60 seconds
I2C Speed: 400kHz
Battery Thresholds:
  - Upper Limit: 3.3V
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
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/esp32c3.jpg" width="100"><br>Xiao ESP32 C3</td>
  <td>
    • MCU: ESP32-C3 RISC-V<br>
    • Clock: 160MHz<br>
    • RAM: 400KB SRAM<br>
    • Flash: 4MB<br>
    • WiFi: 2.4GHz<br>
    • Bluetooth: BLE 5.0<br>
    • GPIO: 11 pins<br>
    • Size: 21x17.5mm<br>
    • Deep Sleep: ~14µA
  </td>
  <td>Ultra-compact IoT controller with low power consumption and built-in wireless capabilities</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/epd.jpg" width="100"><br>Waveshare 3.52" E-Paper</td>
  <td>
    • Resolution: 360x240<br>
    • Colors: BiColor<br>
    • Refresh: 1.5 secs
  </td>
  <td>Display unit with low power consumption</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/bh1750.jpg" width="100"><br>BH1750</td>
  <td>
    • I2C Address: 0x23<br>
    • Mode: ONE_TIME_HIGH_RES<br>
    • Precision: 1-65535 lux
  </td>
  <td>Ambient light sensor for night mode</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/ds3231.jpg" width="100"><br>DS3231</td>
  <td>
    • Accuracy: ±2ppm<br>
    • Temperature comp.<br>
    • Battery backup
  </td>
  <td>Real-time clock for timekeeping</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/battery.jpg" width="100"><br>IFR32650 LiFePO4</td>
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
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/tp5000.jpg" width="100"><br>TP5000</td>
  <td>
    • Input: 5V USB<br>
    • Output: 3.2V<br>
    • Current: Up to 2A
  </td>
  <td>Battery charging module</td>
</tr>
<tr>
  <td><img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/usbc.jpg" width="100"><br>USB-C Breakout</td>
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
    E -->|Yes| F[Check Time]
    F --> G[Deep Sleep]
```

## 🚀 Quick Start
<details>
<summary>Prerequisites</summary>

```yaml
Required Libraries:
  - SPI
  - Wire (I2C @ 400kHz)
  - WiFi
  - NTPClient
  - Preferences
  - BH1750 (0x23)
  - RTClib
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

## 📊 Power Analysis
<table>
<tr>
  <th>Mode</th>
  <th>Duration</th>
  <th>Current Draw</th>
</tr>
<tr>
  <td>Deep Sleep</td>
  <td>60 seconds</td>
  <td>~50µA</td>
</tr>
</table>

## 🔋 Battery Performance
```
Initial: 06/06/2024
Final: 01/11/2024
Runtime: ~4 months 27 days
```

## 📷 Gallery
<div align="center">
<img src="https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/epdClock.jpg" width="500">
</div>

<details>
<summary>📝 Technical Notes</summary>

- ADC Pin for Battery: A0
- Battery Sampling: 4 readings average
- Light Sensor: BH1750 (0x23)
- WiFi Mode: STA
- Display Library: OEM Basic (No GxEPD2)
</details>

---
<div align="center">
<p>Made with ❤️ and ♻️ materials</p>
<p>📜 <i>Distribute freely with attribution</i></p>
</div>

