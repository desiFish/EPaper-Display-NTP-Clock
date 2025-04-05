/*
epdNtpClockV1.ino
Copyright (C) 2024 desiFish

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <SPI.h>
#include "src/EPD_3in52.h"
#include "src/imagedata.h"
#include "src/epdpaint.h"

#include <Wire.h>
#include "RTClib.h"

#include <WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <Preferences.h>
#include <BH1750.h>

// powersave wifi off
#include "esp_wifi.h"

Preferences pref; // preference library object

RTC_DS3231 rtc;          // ds3231 object
BH1750 lightMeter(0x23); // Initalize light sensor

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
int TIME_TO_SLEEP = 60;        // in seconds

#define BATPIN A0                // battery voltage divider connection pin (1M Ohm with 104 Capacitor)
#define BATTERY_LEVEL_SAMPLING 4 // number of times to average the reading

// System configuration constants
#define WIFI_CONNECT_TIMEOUT 10000 // Timeout for WiFi connection attempts (ms)
#define CPU_FREQ_NORMAL 80         // CPU frequency during active operations (MHz)
#define CPU_FREQ_SLEEP 20          // CPU frequency during sleep mode (MHz)

// Battery monitoring thresholds (Volts)
#define battChangeThreshold 0.15 // Minimum voltage change to update reading
#define battUpperLim 3.3         // Upper limit for normal battery operation
#define battHigh 3.4             // Full battery threshold
#define battLow 2.9              // Low battery threshold

const char *ssid = "SonyBraviaX400";
const char *password = "79756622761";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 19800); // 19800 is offset of India, asia.pool.ntp.org is close to India

#define COLORED 0
#define UNCOLORED 1

UBYTE image[68000];
Epd epd;

bool errFlag = false;
/**
 * @brief Measures battery voltage with averaging
 * @return float Actual battery voltage in volts
 * @note Uses multiple samples to reduce noise
 */
float batteryLevel()
{
  uint32_t Vbatt = 0;
  for (int i = 0; i < BATTERY_LEVEL_SAMPLING; i++)
  {
    Vbatt = Vbatt + analogReadMilliVolts(BATPIN); // ADC with correction
    delay(10);
  }
  float Vbattf = 2 * Vbatt / BATTERY_LEVEL_SAMPLING / 1000.0; // attenuation ratio 1/2, mV --> V
  // Serial.println(Vbattf);
  return (Vbattf);
}
// Weekday names for display
static const char daysOfTheWeek[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/**
 * @brief Enables WiFi with power-optimized settings
 * @note Includes timeout and CPU frequency management
 */
void enableWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < WIFI_CONNECT_TIMEOUT)
  {
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    disableWiFi();
  }
}

/**
 * @brief Disables WiFi and other wireless interfaces to save power
 * @note Also configures CPU for low-power operation
 */
void disableWiFi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_wifi_stop();
  btStop();
}

/**
 * @brief Updates RTC time from NTP server if necessary
 *
 * This function checks if an update is needed based on the following criteria:
 * 1. If it's the first run (lastUpdateDay is 0)
 * 2. If 20 days have passed since the last update
 * 3. If a force update is requested
 *
 * It handles month rollovers when calculating days passed.
 * If an update is needed and WiFi is connected, it fetches the current time
 * from an NTP server and updates the RTC.
 *
 * @param forceUpdate If true, bypasses the normal update interval check
 * @return bool Returns true if the time was successfully updated, false otherwise
 * @note Requires an active WiFi connection to function
 * @note Uses Preferences to store the last update day
 */
bool autoTimeUpdate(bool forceUpdate = false)
{
  if (!pref.isKey("lastUpdateDay"))
    pref.putUChar("lastUpdateDay", 0);

  byte lastUpdateDay = pref.getUChar("lastUpdateDay", 0);
  DateTime now = rtc.now();
  byte currentDay = now.day();

  // Calculate days passed, handling month rollover
  int daysPassed = (currentDay - lastUpdateDay + 31) % 31; // Handle month rollover

  // Check if 20 days have passed since last update
  if (lastUpdateDay == 0 || daysPassed >= 20 || forceUpdate)
  {
    enableWiFi();
    if (WiFi.status() == WL_CONNECTED)
    {
      timeClient.begin();
      if (timeClient.update() && timeClient.isTimeSet())
      {
        time_t rawtime = timeClient.getEpochTime();
        struct tm *ti = localtime(&rawtime);

        uint16_t year = ti->tm_year + 1900;
        uint8_t month = ti->tm_mon + 1;
        uint8_t day = ti->tm_mday;

        rtc.adjust(DateTime(year, month, day,
                            timeClient.getHours(),
                            timeClient.getMinutes(),
                            timeClient.getSeconds()));

        // Update last update day
        pref.putUChar("lastUpdateDay", currentDay);
        Serial.println("RTC updated: " + String(year) + "-" +
                       String(month) + "-" + String(day));
        return true;
      }
    }
  }
  return false;
}

/**
 * @brief Pads single digit numbers with leading zero
 * @param num Number to pad
 * @return String Padded number as string
 */
String padNum(int num)
{
  return (num < 10 ? "0" : "") + String(num);
}

/**
 * @brief Main setup function - runs once at startup/wake
 * @note Device enters deep sleep after completing operations
 */
void setup()
{
  setCpuFrequencyMhz(80);
  disableWiFi(); // Initialize peripherals with power-optimized settings
  Serial.begin(115200);
  pinMode(BATPIN, INPUT);
  Wire.begin();
  Wire.setClock(400000); // Set I2C clock speed to 400kHz
  analogReadResolution(12);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    // showMsg("RTC Error");
    errFlag = true;
  }

  if (lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE))
  {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else
  {
    Serial.println(F("Error initialising BH1750"));
    errFlag = true;
  }
  float lux = 0;
  while (!lightMeter.measurementReady(true))
  {
    yield();
  }
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  pref.begin("database", false); // Open Preferences with namespace "database"

  if (!pref.isKey("nightFlag"))
  { // create key:value pair
    pref.putBool("nightFlag", false);
  }
  bool nightFlag = pref.getBool("nightFlag", false); // remembers last state of clock i.e. sleeping or not
  bool tempNightFlag = nightFlag;

  if (lux == 0)
  {
    if (!nightFlag)
    { // prevents unnecessary redrawing of same thing
      nightFlag = true;
      if (epd.Init() != 0)
      {
        Serial.println("e-Paper init failed");
        return;
      }
      epd.Clear();
      if (errFlag)
        showMsg("Error");
      else
        showMsg("SLEEPING");
    }
  }
  else
  {
    nightFlag = false;

    float battLevel;
    if (!pref.isKey("battLevel")) // create key:value pairs
      pref.putFloat("battLevel", 0.0);
    battLevel = pref.getFloat("battLevel", 0.0);

    float tempBattLevel = battLevel;

    if (!pref.isKey("timeNeedsUpdate")) // create key:value pairs
      pref.putBool("timeNeedsUpdate", true);
    bool timeNeedsUpdate = pref.getBool("timeNeedsUpdate", false);

    DateTime now = rtc.now();
    if ((now.year() == 1970) || rtc.lostPower()) // if RTC lost power or not set
      timeNeedsUpdate = true;

    // Get the current day
    byte currentDay = now.day();

    // Check if we need to update time (once per day)
    if (!pref.isKey("lastCheckedDay")) // create key:value pairs
      pref.putUChar("lastCheckedDay", 0);
    byte lastCheckedDay = pref.getUChar("lastCheckedDay", 0);
    if ((lastCheckedDay != currentDay) || timeNeedsUpdate)
    {
      Serial.println("Updating time from NTP server");
      if (autoTimeUpdate(timeNeedsUpdate)) // Update time from NTP server
        Serial.println("Time updated");
      else
        Serial.println("Time Not updated");
      disableWiFi(); // Turn off WiFi to save power
      timeNeedsUpdate = false;
      pref.putBool("timeNeedsUpdate", false);
      lastCheckedDay = currentDay; // Update last checked day
      pref.putUChar("lastCheckedDay", lastCheckedDay);
    }
    else
      Serial.println("Time already updated");

    // Battery level handling
    float newBattLevel = batteryLevel();
    battLevel = (newBattLevel < battLevel) ? newBattLevel : ((newBattLevel - battLevel) >= battChangeThreshold || newBattLevel > battUpperLim) ? newBattLevel
                                                                                                                                               : battLevel;
    int percent = constrain(((battLevel - battLow) / (battHigh - battLow)) * 100, 0, 100);

    if (tempBattLevel != battLevel)
      pref.putFloat("battLevel", battLevel);

    String percentStr;
    if (battLevel >= 4.0)
      percentStr = "USB";
    else
      percentStr = String(percent) + "%";

    // battery level end

    byte tempHour = now.twelveHour();

    byte temp = int(rtc.getTemperature());

    String dateString = padNum(now.day()) + "/" + padNum(now.month()) + "/" + String(now.year());
    String timeString = padNum(tempHour) + ":" + padNum(now.minute()) + (now.isPM() ? " PM" : " AM");
    if (epd.Init() != 0)
    {
      Serial.println("e-Paper init failed");
      return;
    }
    epd.Clear();
    Serial.println("e-Paper Clear");
    if (timeNeedsUpdate)
      showMsg("Time Resync Await");
    else if (errFlag)
      showMsg("Error");
    else
      showTime(daysOfTheWeek[now.dayOfTheWeek()], timeString, dateString, String(battLevel) + "V", percentStr, percent /*for battery icon*/, String(temp) + "C");
  }
  if (tempNightFlag != nightFlag)
    pref.putBool("nightFlag", nightFlag);

  pref.end(); // Close the preferences

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP / 60) + " Mins");
  // Go to sleep now
  Serial.println("Going to sleep now");
  Serial.flush();
  delay(5);
  // esp_deep_sleep_start();
}

void loop()
{
  // This will never run
}

// various error msg display function
/**
 * @brief Displays error or status messages
 * @param msg Message to display
 * @note Optimized for minimum display updates
 */
void showMsg(String msg)
{
  epd.display_NUM(EPD_3IN52_WHITE);
  epd.lut_GC();
  epd.refresh();
  epd.SendCommand(0x50);
  epd.SendData(0x17);
  delay(100);

  Paint paint(image, 240, 360); // width should be the multiple of 8
  paint.SetRotate(3);           // Top right (0,0)
  paint.Clear(COLORED);
  paint.DrawStringAt(10, 100, msg.c_str(), &Font48, UNCOLORED);

  epd.display_part(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.lut_GC();
  epd.refresh();
  Serial.println("sleep......");
  delay(100);
  epd.sleep();
  Serial.println("end");
}

// Displays time, battery info. First para is week in const char, then time in hh:mm am/pm, then date in dd/mm/yyyy, then battlevel in X.YZV, percent in XY%
/**
 * @brief Updates display with time and status information
 * @param w Weekday string (const)
 * @param timeString Formatted time string
 * @param dateString Formatted date string
 * @param battLevel Battery voltage string
 * @param percentStr Battery percentage string
 * @param percent Battery percentage value for icon
 * @param temp Temperature string
 * @note Implements power-efficient display update strategy
 */
void showTime(const char *w, String timeString, String dateString,
              String battLevel, String percentStr, byte percent, String temp)
{
  epd.display_NUM(EPD_3IN52_WHITE);
  epd.lut_GC();
  epd.refresh();
  epd.SendCommand(0x50);
  epd.SendData(0x17);
  delay(100);

  Paint paint(image, 240, 360); // width should be the multiple of 8
  paint.SetRotate(3);           // Top right (0,0)
  paint.Clear(UNCOLORED);

  // Battery icon outline
  paint.DrawRectangle(10, 4, 26, 12, COLORED);
  paint.DrawRectangle(8, 6, 10, 10, COLORED);

  // Battery fill level
  byte fillX = 25; // Default to empty (rightmost position)
  if (percent > 0)
  {
    if (percent >= 95)
      fillX = 11; // Full
    else if (percent >= 85)
      fillX = 13; // Full-Med
    else if (percent >= 70)
      fillX = 15; // Med
    else if (percent > 50)
      fillX = 17; // Med-half
    else if (percent > 30)
      fillX = 19; // Half
    else if (percent > 10)
      fillX = 21; // Low-half
    else if (percent > 5)
      fillX = 23; // Low
    else if (percent > 2)
      fillX = 25; // Critical

    paint.DrawFilledRectangle(fillX, 4, 25, 11, COLORED);
  }

  paint.DrawStringAt(325, 5, percentStr.c_str(), &Font12, COLORED);
  paint.DrawStringAt(280, 5, battLevel.c_str(), &Font12, COLORED);
  paint.DrawStringAt(60, 30, w, &Font48, COLORED);
  paint.DrawStringAt(295, 50, temp.c_str(), &Font24, COLORED);
  paint.DrawStringAt(327, 47, "o", &Font8, COLORED);
  paint.DrawStringAt(60, 100, timeString.c_str(), &Font48, COLORED);
  paint.DrawStringAt(60, 170, dateString.c_str(), &Font48, COLORED);

  epd.display_part(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.lut_GC();
  epd.refresh();
  Serial.println("sleep......");
  delay(100);
  epd.sleep();
  Serial.println("end");
}