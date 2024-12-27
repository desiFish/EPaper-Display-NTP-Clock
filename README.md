# Minimalistic E-Paper 3.52inch Display NTP-RTC Clock

## Hardware
| Component                        | Description                                      |
|----------------------------------|--------------------------------------------------|
| Xiao ESP32 C3                    | Microcontroller                                  |
| Waveshare 3.52inch BiColor E-paper | 360x240 display (refresh time: 1.5 secs)         |
| BH1750                           | Light sensor                                     |
| DS3231                           | RTC for timekeeping                              |
| LiFePO4 6000mAh 3.2V Battery     | Cheaper compared to ICR Cells                    |
| TP5000                           | 2-in-1 charging module                           |
| BMS                              | 2.5V Low Cutoff                                  |
| USB-C breakout                   | Any receiving board                              |
| Other components                 | Wire, capacitor (104), resistor (1M Ohm), connectors, switch, LED (1), etc. |

## Notes
1. Update the GPIO pin config in the header file `src/epdif.h`.

## Getting Started
### Prerequisites
- Arduino IDE installed
- ESP32 board support added to Arduino IDE
- Required libraries: `SPI`, `Wire`, `WiFi`, `NTPClient`, `Preferences`, `BH1750`, `RTClib`

### Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock.git
   ```
2. Open the project in Arduino IDE.
3. Update the WiFi credentials in the `epdNtpClockV1.ino` file:
   ```cpp
   const char *ssid = "Your_SSID";
   const char *password = "Your_PASSWORD";
   ```
4. Upload the code to your ESP32 board.

## Upcoming Features
Check [issues](https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/issues).

Clock based on Waveshare 3.52inch e-Paper HAT, 360 × 240. :leaves: Eco-Friendly!

The clock runs on a 6000mAh LiFePO4 cell (:leaves: :leaves: Eco-Friendly!). It uses an RTC for power-saving operation and connects to NTP daily to update itself.

It also includes a LUX sensor (BH1750) for sleeping while it is dark (E-paper doesn't have a backlight). (:leaves: :leaves: :leaves: Eco-Friendly!)

Made of old delivery cardboards. :exploding_head: One-Punch Eco-Friendly Boost Ultra Pro Max :leaves: to :infinity:

Supports 5V 1A charging with options up to 2A.

All USB-C operation (charging and debugging).

This clock does not run on the GxEPD2 Library (the display was not supported at the time of making). It uses the OEM-provided basic library.

Battery life:
- Full: 06/06/2024
- Dead: 01/11/2024 (Around 4 months and 27 days)

## Pictures

![Clock](https://github.com/KamadoTanjiro-beep/E-Paper-Display-NTP-Clock/blob/main/src/epdClock.jpg)

## Features V1.0.1
1. Battery Icon

## Features V1.0.0
| Feature                          | Description                                      |
|----------------------------------|--------------------------------------------------|
| Minimalistic design              | Simple and clean design                          |
| Auto-updates internal clock      | Daily updates (requires Wi-Fi connectivity)      |
| Big Icons                        | Easy to read                                     |
| Battery percentage and voltage   | Displays battery status                          |
| Updates every 60 seconds         | Sleeps for the rest of the time                  |
| Mammoth 6000mAh battery          | Long-lasting power (your personal choice though) |

## License

Distribute it freely but link back to this project or provide some good words or attributes or donate. You are on your own; I take no responsibility if this device causes any damage.

